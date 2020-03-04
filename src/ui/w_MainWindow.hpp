﻿#pragma once

#include "common/HTTPRequestHelper.hpp"
#include "common/LogHighlighter.hpp"
#include "components/pac/QvPACHandler.hpp"
#include "components/speedchart/speedwidget.hpp"
#include "core/handler/ConfigHandler.hpp"
#include "ui/messaging/QvMessageBus.hpp"
#include "ui_w_MainWindow.h"

#include <QMainWindow>
#include <QMenu>
#include <QScrollBar>
#include <QSystemTrayIcon>

// ==========================================================================================
#include "ui/widgets/ConnectionInfoWidget.hpp"
#include "ui/widgets/ConnectionItemWidget.hpp"

enum MW_ITEM_COL
{
    MW_ITEM_COL_DISPLAYNAME = 1,
    MW_ITEM_COL_LATENCY = 2,
    MW_ITEM_COL_IMPORTTIME = 3,
    MW_ITEM_COL_LASTCONNETED = 4,
    MW_ITEM_COL_DATAUSAGE = 5
};

class MainWindow
    : public QMainWindow
    , Ui::MainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
  signals:
    void StartConnection() const;
    void StopConnection() const;
    void RestartConnection() const;
  public slots:
    QvMessageBusSlotDecl;
  private slots:
    void on_activatedTray(QSystemTrayIcon::ActivationReason reason);
    void on_actionExit_triggered();
    void on_preferencesBtn_clicked();
    void on_clearlogButton_clicked();
    void on_connectionListWidget_customContextMenuRequested(const QPoint &pos);
    void on_importConfigButton_clicked();
    void on_subsButton_clicked();
    //
    void ToggleVisibility();
#ifndef DISABLE_AUTO_UPDATE
    void VersionUpdate(QByteArray &data);
#endif
    //

  public:
    static MainWindow *mwInstance;

  protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void closeEvent(QCloseEvent *) override;

  private slots:
    void OnEditRequested(const ConnectionId &id);
    void OnEditJsonRequested(const ConnectionId &id);
    //
    void OnConnectionWidgetFocusRequested(const ConnectionItemWidget *widget);
    //
    void OnConnected(const ConnectionId &id);
    void OnDisconnected(const ConnectionId &id);
    //
    void OnStatsAvailable(const ConnectionId &id, const quint64 upS, const quint64 downS, const quint64 upD, const quint64 downD);
    void OnVCoreLogAvailable(const ConnectionId &id, const QString &log);
    //
    void OnConnectionCreated(const ConnectionId &id, const QString &displayName);
    void OnConnectionDeleted(const ConnectionId &id, const GroupId &groupId);
    void OnConnectionGroupChanged(const ConnectionId &id, const GroupId &originalGroup, const GroupId &newGroup);
    //
    void OnGroupCreated(const GroupId &id, const QString &displayName);
    void OnGroupDeleted(const GroupId &id, const QList<ConnectionId> &connections);
    //
    void on_action_StartThis_triggered();
    void on_action_RCM_EditThis_triggered();
    void on_action_RCM_EditAsJson_triggered();
    void on_action_RCM_EditAsComplex_triggered();
    void on_action_RCM_RenameThis_triggered();
    void on_action_RCM_DeleteThese_triggered();
    void on_action_RCM_DuplicateThese_triggered();
    //
    void on_connectionListWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_connectionFilterTxt_textEdited(const QString &arg1);
    void on_connectionListWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_locateBtn_clicked();
    //
    void SortConnectionList(MW_ITEM_COL byCol, bool asending);

  private:
    QHash<GroupId, shared_ptr<QTreeWidgetItem>> groupNodes;
    QHash<ConnectionId, shared_ptr<QTreeWidgetItem>> connectionNodes;
    //
    QTreeWidgetItem *CurrentItem;
    // Charts
    SpeedWidget *speedChartWidget;
    QMenu *connectionListMenu;
#ifndef DISABLE_AUTO_UPDATE
    QvHttpRequestHelper *requestHelper;
#endif
    QSystemTrayIcon hTray;
    PACServer pacServer;
    // QvTCPingModel tcpingHelper;
    SyntaxHighlighter *vCoreLogHighlighter;
    ConnectionInfoWidget *infoWidget;
    //
    // Actions in the system tray menu
    QMenu *tray_RootMenu = new QMenu(this);
    QAction *action_Tray_ShowHide = new QAction(this->windowIcon(), tr("Hide"), this);
    QAction *action_Tray_ShowPreferencesWindow = new QAction(tr("Preferences"), this);
    QAction *action_Tray_Quit = new QAction(tr("Quit"), this);
    // --> Connectivities
    QAction *action_Tray_Start = new QAction(tr("Connect"), this);
    QAction *action_Tray_Restart = new QAction(tr("Reconnect"), this);
    QAction *action_Tray_Stop = new QAction(tr("Disconnect"), this);
    // --> System proxy settings
    QMenu *tray_SystemProxyMenu = new QMenu(this);
    QAction *action_Tray_SetSystemProxy = new QAction(tr("Enable System Proxy"), this);
    QAction *action_Tray_ClearSystemProxy = new QAction(tr("Disable System Proxy"), this);
    //
    ConnectionId lastConnectedId;
    void MWSetSystemProxy();
    void CheckSubscriptionsUpdate();
    //
    void MWAddConnectionItem_p(const ConnectionId &connection, const GroupId &groupId);
    void MWAddGroupItem_p(const GroupId &groupId);
};
