/*
 * A238148  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A238148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over consecutive pairs of rows
   using the constraint on 2×2 subblocks.  The table T[a][b] contains
   max(a,b)+secondLargest(a,b) for rows a and b.  The DP state is a
   pair (prev,curr).  Transition from (prev,curr) to (curr,next) is
   allowed iff |T[prev][curr] − T[curr][next]| == 1.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External types supplied by build */
typedef uint64_t idx_t;       /* will be overridden if necessary */

/* State structure */
typedef struct {
    seq_t *cur;          /* current DP vector (size 256) */
    seq_t *next;         /* next DP vector (size 256) */
    seq_t *cache;        /* cache of a(k) values (k >= 1) */
    idx_t cachelen;      /* number of terms cached */
    idx_t cachecap;      /* capacity of cache array */
} dp_state_t;

static dp_state_t dp = {0,0,0,0};

/* Precomputed table T[a][b] = max+second_largest of the 2×2 block
   formed by rows a and b (rows encoded as 4 bits). */
static unsigned char T[16][16];

static void compute_T_matrix(void)
{
    for(int a=0; a<16; ++a) {
        int a0 = a & 3;            /* first column (bits 0-1) */
        int a1 = (a>>2) & 3;       /* second column (bits 2-3) */
        for(int b=0; b<16; ++b) {
            int b0 = b & 3;
            int b1 = (b>>2) & 3;
            int vals[4] = {a0,a1,b0,b1};
            int mx = -1, smax = -1;
            for(int i=0; i<4; ++i) {
                int v = vals[i];
                if(v > mx) {
                    smax = mx;
                    mx = v;
                } else if(v > smax) {
                    smax = v;
                }
            }
            T[a][b] = (unsigned char)(mx + smax);
        }
    }
}

static void dp_init(idx_t nmax)
{
    if(dp.cur) return;   /* already initialized */

    compute_T_matrix();

    dp.cur  = (seq_t*)malloc(sizeof(seq_t)*256);
    dp.next = (seq_t*)malloc(sizeof(seq_t)*256);
    for(int i=0; i<256; ++i) dp.cur[i] = (seq_t)1;   /* all (prev,curr) pairs for a 2×2 array */

    dp.cachecap = (idx_t)nmax;
    dp.cache = (seq_t*)malloc(sizeof(seq_t)*dp.cachecap);
    dp.cachelen = 0;
}

static void dp_ensure_cache(idx_t needed)
{
    if(dp.cachecap >= needed) return;
    dp.cache = (seq_t*)realloc(dp.cache, sizeof(seq_t)*needed);
    dp.cachecap = needed;
}

static void dp_compute_upto(idx_t max_n)
{
    dp_ensure_cache(max_n);
    dp_init(max_n);

    for(idx_t n = dp.cachelen + 1; n <= max_n; ++n) {
        if(n == 1) {
            dp.cache[0] = (seq_t)256;   /* 16 × 16 arrays */
            dp.cachelen = 1;
            continue;
        }

        /* Transition to build row n (total rows = n+1) */
        memset(dp.next, 0, sizeof(seq_t)*256);
        for(int v=0; v<256; ++v) {
            seq_t curval = dp.cur[v];
            if(curval == (seq_t)0) continue;
            int prev = v/16;
            int curr = v%16;
            unsigned char t_prev_curr = T[prev][curr];
            for(int nxt=0; nxt<16; ++nxt) {
                if(abs((int)t_prev_curr - (int)T[curr][nxt]) == 1) {
                    int idx = curr*16 + nxt;   /* state (curr,nxt) */
                    dp.next[idx] += curval;
                }
            }
        }

        /* Sum all states to get a(n) */
        seq_t sum = 0;
        for(int i=0; i<256; ++i) sum += dp.next[i];
        dp.cache[n-1] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = dp.cur;
        dp.cur = dp.next;
        dp.next = tmp;

        dp.cachelen = n;
    }
}

/* External function required by tests */
seq_t a(idx_t n)
{
    if(n == 0) return (seq_t)0;       /* undefined */
    dp_compute_upto(n);
    return dp.cache[n-1];
}
