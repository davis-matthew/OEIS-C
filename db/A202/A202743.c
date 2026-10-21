/*
 * A202743  Number of 2Xn 0..1 arrays with row sums equal and column sums
 *          unequal to adjacent columns.
 * https://oeis.org/A202743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 2×n 0..1 arrays with equal row sums and column sums
 *            unequal to adjacent columns. * * The algorithm uses dynamic
 *            programming over the last column sum (0,1,2) and * the current
 *            difference between total 1's in the two rows. For a column with
 *            * sum 0 or 2 no change occurs; a column with sum...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Defined only for n >= 1 */
    if (n == 1) return 2;                 /* Base case, handled early */

    const idx_t offset = n;                /* Index offset for zero difference */
    const size_t D     = (size_t)(2 * n + 1);  /* Number of possible diff values */

    /* Allocate two layers of DP tables.  Memory is freed at the end. */
    seq_t *cur[3];
    seq_t *next[3];
    for (int s = 0; s < 3; ++s) {
#ifdef __cplusplus
        cur[s]  = new seq_t[D]();           /* value‑initialized to 0 in C++ */
        next[s] = new seq_t[D]();
#else
        cur[s]  = (seq_t *)calloc(D, sizeof(seq_t)); /* zero‑initialized in C */
        next[s] = (seq_t *)calloc(D, sizeof(seq_t));
#endif
        if (!cur[s] || !next[s]) {          /* Allocation failed */
            for (int t = 0; t < s; ++t) {
#ifdef __cplusplus
                delete[] cur[t];
                delete[] next[t];
#else
                free(cur[t]);
                free(next[t]);
#endif
            }
            return 0;
        }
    }

    /* Zero the current layer (needed for C where calloc already did it). */
    for (int s = 0; s < 3; ++s)
        for (size_t i = 0; i < D; ++i)
            cur[s][i] = 0;

    /* Initial column: only one pattern for each sum.  Two patterns for sum 1. */
    cur[0][offset]      = 1;      /* Column sum 0, diff 0   */
    cur[2][offset]      = 1;      /* Column sum 2, diff 0   */
    cur[1][offset + 1]  = 1;      /* Column sum 1, pattern 1,0  (diff +1) */
    cur[1][offset - 1]  = 1;      /* Column sum 1, pattern 0,1  (diff –1) */

    /* DP over remaining columns (n-1 steps). */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero the next layer. */
        for (int s = 0; s < 3; ++s)
            for (size_t i = 0; i < D; ++i)
                next[s][i] = 0;

        const int d_start = offset - (int)step;
        const int d_end   = offset + (int)step;

        for (int prev_s = 0; prev_s < 3; ++prev_s) {
            for (int d = d_start; d <= d_end; ++d) {
                seq_t val = cur[prev_s][(size_t)d];
                if (!val) continue;

                for (int next_s = 0; next_s < 3; ++next_s) {
                    if (next_s == prev_s) continue;

                    if (next_s == 0 || next_s == 2) {
                        /* Sum 0 or 2: difference unchanged. */
                        next[next_s][(size_t)d] += val;
                    } else {  /* next_s == 1 */
                        /* Sum 1: two patterns ±1. */
                        next[1][(size_t)(d + 1)] += val;
                        next[1][(size_t)(d - 1)] += val;
                    }
                }
            }
        }

        /* Swap current and next layers. */
        for (int s = 0; s < 3; ++s) {
            seq_t *tmp = cur[s];
            cur[s] = next[s];
            next[s] = tmp;
        }
    }

    /* Sum over all ending sums with diff zero. */
    const size_t idx = offset;
    seq_t result = cur[0][idx] + cur[1][idx] + cur[2][idx];

    /* Free allocated memory. */
    for (int s = 0; s < 3; ++s) {
#ifdef __cplusplus
        delete[] cur[s];
        delete[] next[s];
#else
        free(cur[s]);
        free(next[s]);
#endif
    }

    return result;
}
