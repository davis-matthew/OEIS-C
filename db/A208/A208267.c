/*
 * A208267  Number of nX6 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with the left‑most cell * 0
 *            and no 2×2 block of equal bits. Computed by dynamic programming
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_realloc.h"

#define COLS   6
#define STATES (1 << COLS)

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Transition table: allowed[p][q] == 1 iff no 2×2 block of equal bits
       appears between rows p (previous) and q (current).  */
    static unsigned char allowed[STATES][STATES];
    static int init = 0;
    if (!init) {
        for (int p = 0; p < STATES; ++p) {
            for (int q = 0; q < STATES; ++q) {
                unsigned char ok = 1;
                for (int j = 0; j < COLS - 1; ++j) {
                    unsigned char b0 = (p >> j) & 1U;
                    unsigned char b1 = (p >> (j + 1)) & 1U;
                    unsigned char c0 = (q >> j) & 1U;
                    unsigned char c1 = (q >> (j + 1)) & 1U;
                    if (b0 == b1 && b0 == c0 && b0 == c1) {
                        ok = 0;
                        break;
                    }
                }
                allowed[p][q] = ok;
            }
        }
        init = 1;
    }

    /* DP arrays for counts of rows.  */
    static seq_t prev[STATES];
    static seq_t cur[STATES];

    /* Cache of previously computed a(m). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of slots allocated (>=1). */
    static idx_t last_computed = 0;

    /* Allocate or grow cache using seq_realloc (supports arbitrary‑precision seq_t). */
    if (n + 1 > cache_len) {
        cache = seq_realloc(cache, (size_t)cache_len, (size_t)(n + 1));
        cache_len = n + 1;
    }

    /* Compute values up to n if not already done. */
    for (idx_t step = last_computed + 1; step <= n; ++step) {
        seq_t sum = 0;

        if (step == 1) {
            /* First row: leftmost bit fixed to 0. */
            for (int s = 0; s < STATES; ++s) {
                if ((s & 1) == 0) {
                    prev[s] = 1;
                } else {
                    prev[s] = 0;
                }
                sum += prev[s];
            }
        } else {
            /* Transition from prev to cur. */
            for (int q = 0; q < STATES; ++q) cur[q] = 0;
            for (int p = 0; p < STATES; ++p) {
                if (prev[p] == 0) continue;
                for (int q = 0; q < STATES; ++q) {
                    if (!allowed[p][q]) continue;
                    cur[q] += prev[p];
                }
            }
            /* Sum over all states for current row and copy to prev for next step. */
            for (int q = 0; q < STATES; ++q) {
                sum += cur[q];
                prev[q] = cur[q];
            }
        }
        cache[step] = sum;
    }

    last_computed = n;
    return cache[n];
}
