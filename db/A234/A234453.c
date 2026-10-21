/*
 * A234453  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) counts (n+1)×3 arrays over {0,…,5} whose every 2×2 subblock
   satisfies (a00 + a11) – (a01 + a10) = ±1 and (a01 + a12) – (a02 + a11) = ±1.
   We model each row as a state (a0,a1,a2) – 6^3 = 216 possibilities.
   For each pair of consecutive rows we pre‑compute all admissible transitions.
   The number of arrays equals the total number of length‑(n+1) paths in this
   directed graph.  The algorithm runs in O(n·E) time where E=2400 is the
   number of admissible transitions.  It works for any n≥1 and any seq_t
   integer type. */

/* ------------------------------------------------------------ */
/* You must have the following typedefs elsewhere:
   typedef ... seq_t; // unsigned integer type, possibly big‑int
   typedef ... idx_t; // unsigned integer type for indices
   ------------------------------------------------------------ */

#include <stddef.h>
#include <stdint.h>

#ifndef MAX_STATES
#define MAX_STATES 216
#endif

/* Pre‑computed adjacency information */
static idx_t  adjacency[MAX_STATES][256];
static uint8_t deg[MAX_STATES];
static int    adj_built = 0;

/* Build adjacency list once */
static void build_adjacency(void)
{
    if (adj_built) return;
    for (int s = 0; s < MAX_STATES; ++s) {
        int r0 = s / 36;
        int r1 = (s / 6) % 6;
        int r2 = s % 6;
        deg[s] = 0;
        for (int b0 = 0; b0 < 6; ++b0) {
            for (int d0 = -1; d0 <= 1; d0 += 2) {
                int b1 = r1 + b0 - r0 + d0;
                if (b1 < 0 || b1 > 5) continue;
                for (int d1 = -1; d1 <= 1; d1 += 2) {
                    int b2 = r2 + b1 - r1 + d1;
                    if (b2 < 0 || b2 > 5) continue;
                    int t = b0 * 36 + b1 * 6 + b2;
                    adjacency[s][deg[s]++] = (idx_t)t;
                }
            }
        }
    }
    adj_built = 1;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; but n=0 is undefined */
    build_adjacency();

    seq_t dp[MAX_STATES], ndp[MAX_STATES];
    for (int i = 0; i < MAX_STATES; ++i) dp[i] = 1; /* first row arbitrary */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < MAX_STATES; ++i) ndp[i] = 0;
        for (int s = 0; s < MAX_STATES; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            uint8_t d = deg[s];
            for (uint8_t k = 0; k < d; ++k) {
                int t = (int)adjacency[s][k];
                ndp[t] += val;
            }
        }
        for (int i = 0; i < MAX_STATES; ++i) dp[i] = ndp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < MAX_STATES; ++i) total += dp[i];
    return total;
}
