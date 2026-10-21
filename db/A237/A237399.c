/*
 * A237399  Number of (n+1) X (1+1) 0..3 arrays with the maximum plus the
 *          upper median plus the lower median minus the minimum of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   init_sMap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A237399.
   The algorithm directly follows the definition:
   Count (n+1)×2 0..3 arrays such that the value
   max+upper median+lower median−min of each 2×2 subblock
   differs from that of its vertical neighbour by exactly 1.
   This is equivalent to counting walks of length n in a graph
   whose vertices are the 16 possible row pairs (2 cells);
   an edge connects rows r1→r2 iff the associated block
   values differ by +/-1.
   The DP below computes the number of such walks for any n≥1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/init_sMap.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    /* Static state */
    static bool initialized = false;
    static int sMap[16][16];
    static seq_t dp_cur[16][10];
    static seq_t dp_next[16][10];
    static seq_t *values = NULL;
    static size_t values_len = 0;   /* number of computed a(k), k>=1 */
    static size_t values_cap = 0;

    if (!initialized) {
        init_sMap(sMap);
        /* compute base case a(1) */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 10; ++j)
                dp_cur[i][j] = (seq_t)0;

        for (int r0 = 0; r0 < 16; ++r0)
            for (int r1 = 0; r1 < 16; ++r1) {
                int s = sMap[r0][r1];
                dp_cur[r1][s] += (seq_t)1;
            }

        seq_t total = (seq_t)0;
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 10; ++j)
                total += dp_cur[i][j];

        values_cap = 16; /* initial capacity */
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) exit(1);
        values[0] = total;
        values_len = 1;
        initialized = true;
    }

    /* Extend precomputation up to n */
    while (values_len < n) {
        /* compute nextdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 10; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int prev_row = 0; prev_row < 16; ++prev_row) {
            for (int prev_s = 0; prev_s < 10; ++prev_s) {
                seq_t cnt = dp_cur[prev_row][prev_s];
                if (!cnt) continue;
                for (int next_row = 0; next_row < 16; ++next_row) {
                    int cur_s = sMap[prev_row][next_row];
                    int diff = cur_s - (int)prev_s;
                    if (diff == 1 || diff == -1) {
                        dp_next[next_row][cur_s] += cnt;
                    }
                }
            }
        }

        /* total count for this n */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 10; ++j)
                total += dp_next[i][j];

        /* store the result */
        if (values_len == values_cap) {
            size_t new_cap = values_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            values = tmp;
            values_cap = new_cap;
        }
        values[values_len] = total;
        ++values_len;

        /* swap dp buffers */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 10; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    return values[n - 1];
}
