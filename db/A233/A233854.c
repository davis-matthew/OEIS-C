/*
 * A233854  Number of (n+1)X(2+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 59.
 * https://oeis.org/A233854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+1)×3 arrays with entries 0..7 such that every 2×2 subblock has the
 * sum of the squares of all six edge and diagonal differences equal to 59.
 * The function computes the n-th term of OEIS A233854.  It uses dynamic
 * programming with a small state space (512 states for each row).  The
 * transition graph is built once; each call then propagates a walk of
 * length n over this graph.  The algorithm runs in O(n·E) where E≈1040
 * is the number of compatible row pairs, easily fast enough for n up to
 * 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int build_done = 0;
static unsigned short succ[512][512];
static unsigned short succ_cnt[512];

/* Check whether a 2×2 block with corners a,b,c,d satisfies the condition.
 * a,b are the top row, c,d bottom row.  All values are in 0..7. */
static inline int block_ok(int a, int b, int c, int d)
{
    int d1 = b - a; if (d1 < 0) d1 = -d1;
    int d2 = d - c; if (d2 < 0) d2 = -d2;
    int d3 = c - a; if (d3 < 0) d3 = -d3;
    int d4 = d - b; if (d4 < 0) d4 = -d4;
    int d5 = d - a; if (d5 < 0) d5 = -d5;
    int d6 = c - b; if (d6 < 0) d6 = -d6;
    int sum = d1*d1 + d2*d2 + d3*d3 + d4*d4 + d5*d5 + d6*d6;
    return sum == 59;
}

/* Build the transition lists once. */
static void build_adjacency(void)
{
    if (build_done) return;
    for (unsigned int s = 0; s < 512; ++s) {
        int a = s / 64;
        int b = (s / 8) % 8;
        int c = s % 8;
        succ_cnt[s] = 0;
        for (unsigned int t = 0; t < 512; ++t) {
            int A = t / 64;
            int B = (t / 8) % 8;
            int C = t % 8;
            if (block_ok(a, b, A, B) && block_ok(b, c, B, C))
                succ[s][succ_cnt[s]++] = (unsigned short)t;
        }
    }
    build_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, return 0 */
    build_adjacency();

    /* dp arrays */
    seq_t dp[512];
    seq_t ndp[512];
    for (int i = 0; i < 512; ++i) {
        dp[i] = (seq_t)1;   /* first row can be any state */
        ndp[i] = (seq_t)0;
    }

    for (idx_t step = 1; step <= n; ++step) {
        /* zero ndp quickly using loop */
        for (int i = 0; i < 512; ++i) ndp[i] = (seq_t)0;

        for (int s = 0; s < 512; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            unsigned short *arr = succ[s];
            unsigned short k = succ_cnt[s];
            for (unsigned short j = 0; j < k; ++j) {
                ndp[arr[j]] += val;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 512; ++i) dp[i] = ndp[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 512; ++i) total += dp[i];
    return total;
}
