/*
 * A237689  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×3 arrays with entries 0..3 in which for every
 *            2×2 subblock the quantity max + upper_median + min –
 *            lower_median has the same value over all subblocks
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   compute_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a directed graph of 64 row patterns.
   Two rows are connected (directed) if the two corresponding
   2×2 subblocks have equal value K (0≤K≤9).
   For a fixed K, the number of arrays of length n+1 equals the
   number of directed walks of length n in the graph K.
   The total is the sum over K. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_exp.h"

seq_t
a(idx_t n)
{
    /* adjacency lists: adj_to[k][i][t] = j, adj_cnt[k][i] = number of j's */
    static unsigned char adj_cnt[10][64];
    static unsigned char adj_to[10][64][64];
    static uint8_t rows[64][3];
    static bool init = false;

    if (!init) {
        /* fill row patterns */
        for (unsigned int r = 0; r < 64; ++r) {
            rows[r][0] = r & 3;
            rows[r][1] = (r >> 2) & 3;
            rows[r][2] = (r >> 4) & 3;
        }
        /* build directed edges */
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                int e0 = compute_exp(rows[i], rows[j], 0);
                int e1 = compute_exp(rows[i], rows[j], 1);
                if (e0 == e1) {
                    unsigned char k = (unsigned char)e0;
                    adj_to[k][i][adj_cnt[k][i]++] = (unsigned char)j;
                }
            }
        }
        init = true;
    }

    seq_t result = 0;
    seq_t dp[64];
    seq_t nxt[64];

    for (int k = 0; k <= 9; ++k) {
        /* start: any first row is allowed */
        for (int i = 0; i < 64; ++i) dp[i] = 1;

        /* walk of length n */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 64; ++i) nxt[i] = 0;
            for (int i = 0; i < 64; ++i) {
                seq_t val = dp[i];
                if (val == 0) continue;
                unsigned char cnt = adj_cnt[k][i];
                for (unsigned char t = 0; t < cnt; ++t) {
                    unsigned char j = adj_to[k][i][t];
                    nxt[j] += val;
                }
            }
            for (int i = 0; i < 64; ++i) dp[i] = nxt[i];
        }

        /* sum of all walks of length n for this k */
        seq_t subtotal = 0;
        for (int i = 0; i < 64; ++i) subtotal += dp[i];
        result += subtotal;
    }

    return result;
}
