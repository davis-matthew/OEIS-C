/*
 * A210385  Number of (n+1)X4 0..2 arrays with every 2X2 subblock having one,
 *          two or three distinct clockwise edge differences.
 * https://oeis.org/A210385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+1)x4 arrays with entries 0..2 such that every 2x2
 *            subblock has 1,2 or3 distinct clockwise edge differences.
 *            Computed by dynamic programming over row states
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
static bool tr[81][81];
static bool tr_inited = false;
static void init_transitions(void)
{
    int pow3[4]={1,3,9,27};
    int rows[81][4];
    for (int r=0;r<81;r++){
        int x=r;
        for(int c=0;c<4;c++){
            rows[r][c]=x%3;
            x/=3;
        }
    }
    for(int i=0;i<81;i++){
        for(int j=0;j<81;j++){
            bool ok=true;
            for(int col=0;col<3;col++){
                int d0 = rows[i][col]-rows[i][col+1];
                int d1 = rows[i][col+1]-rows[j][col+1];
                int d2 = rows[j][col+1]-rows[j][col];
                int d3 = rows[j][col]-rows[i][col];
                if(d0!=d1 && d0!=d2 && d0!=d3 &&
                   d1!=d2 && d1!=d3 &&
                   d2!=d3){
                    ok=false;
                    break;
                }
            }
            tr[i][j]=ok;
        }
    }
    tr_inited=true;
}
seq_t a(idx_t n)
{
    if(!tr_inited) init_transitions();
    /* number of rows is n+1; we need n transitions */
    seq_t cur[81], nxt[81];
    for(int i=0;i<81;i++){
        cur[i]=1;
    }
    for(idx_t step=0;step<n;step++){
        for(int i=0;i<81;i++) nxt[i]=0;
        for(int prev=0;prev<81;prev++){
            if(cur[prev]==0) continue;
            seq_t val = cur[prev];
            for(int curst=0;curst<81;curst++){
                if(tr[prev][curst]){
                    nxt[curst] = nxt[curst] + val;
                }
            }
        }
        for(int i=0;i<81;i++) cur[i]=nxt[i];
    }
    seq_t total=0;
    for(int i=0;i<81;i++) total = total + cur[i];
    return total;
}
