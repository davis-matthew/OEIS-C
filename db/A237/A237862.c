/*
 * A237862  Number of (n+1)X(2+1) 0..2 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming: matrix‑valued states for
 *            all 2×3 row pairs
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   subblock_stat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/subblock_stat.h"

static const int rows[27][3] = {
    {0,0,0},{0,0,1},{0,0,2},
    {0,1,0},{0,1,1},{0,1,2},
    {0,2,0},{0,2,1},{0,2,2},
    {1,0,0},{1,0,1},{1,0,2},
    {1,1,0},{1,1,1},{1,1,2},
    {1,2,0},{1,2,1},{1,2,2},
    {2,0,0},{2,0,1},{2,0,2},
    {2,1,0},{2,1,1},{2,1,2},
    {2,2,0},{2,2,1},{2,2,2}
};

seq_t a(idx_t n)
{
    enum {R=27, STATE=R*R};

    static bool init=false;
    static bool pair_valid[STATE];
    static unsigned char pair_s0[STATE];
    static unsigned char pair_s1[STATE];
    static unsigned char trans_cnt[STATE];
    static uint8_t trans_adj[STATE][27];   /* at most 27 outgoing edges per state */

    if (!init){
        /* First pass: compute statistics for every pair of rows. */
        for (int a=0;a<R;++a){
            for (int b=0;b<R;++b){
                int idx=a*R+b;
                int s0=subblock_stat(rows[a][0],rows[a][1],rows[b][0],rows[b][1]);
                int s1=subblock_stat(rows[a][1],rows[a][2],rows[b][1],rows[b][2]);
                if (abs(s0-s1)==1){
                    pair_valid[idx]=true;
                    pair_s0[idx]=(unsigned char)s0;
                    pair_s1[idx]=(unsigned char)s1;
                }else{
                    pair_valid[idx]=false;
                }
            }
        }
        /* Second pass: build transitions. */
        for (int a=0;a<R;++a){
            for (int b=0;b<R;++b){
                int idx=a*R+b;
                if (!pair_valid[idx]){ trans_cnt[idx]=0; continue; }
                unsigned char cnt=0;
                for (int c=0;c<R;++c){
                    int idx_next=b*R+c;
                    if (!pair_valid[idx_next]) continue;
                    if (abs((int)pair_s0[idx]-(int)pair_s0[idx_next])==1 &&
                        abs((int)pair_s1[idx]-(int)pair_s1[idx_next])==1){
                        trans_adj[idx][cnt++] = (uint8_t)c;
                    }
                }
                trans_cnt[idx]=cnt;
            }
        }
        init=true;
    }

    /* DP arrays */
    seq_t dp[STATE];
    seq_t next[STATE];
    for (int i=0;i<STATE;++i) dp[i]=0;

    /* Initial pairs: first two rows */
    for (int a=0;a<R;++a){
        for (int b=0;b<R;++b){
            int idx=a*R+b;
            if (pair_valid[idx]) dp[idx]=1;
        }
    }

    if (n==1){
        seq_t ans=0;
        for (int i=0;i<STATE;++i) ans+=dp[i];
        return ans;
    }

    for (idx_t step=1; step<n; ++step){
        for (int i=0;i<STATE;++i) next[i]=0;
        for (int a=0;a<R;++a){
            for (int b=0;b<R;++b){
                int idx=a*R+b;
                if (!pair_valid[idx]) continue;
                seq_t cur=dp[idx];
                if (!cur) continue;
                unsigned char cnt=trans_cnt[idx];
                for (unsigned char t=0; t<cnt; ++t){
                    int c=trans_adj[idx][t];
                    int dest=b*R+c;
                    next[dest]+=cur;
                }
            }
        }
        for (int i=0;i<STATE;++i) dp[i]=next[i];
    }

    seq_t ans=0;
    for (int i=0;i<STATE;++i) ans+=dp[i];
    return ans;
}
