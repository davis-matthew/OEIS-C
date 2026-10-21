/*
 * A237854  Number of (n+1)X(3+1) 0..1 arrays with no element greater than all
 *          horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A237854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 binary arrays with no element greater
 *            than all horizontal neighbors or less than all vertical
 *            neighbors
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* All 4‑bit row patterns that satisfy the horizontal rule 0‑1‑0 is forbidden. */
    const idx_t H[] = {0x0, 0x3, 0x6, 0x7, 0xC, 0xE, 0xF};
    const size_t K = sizeof(H)/sizeof(H[0]);

    seq_t dp_prev[K][K];
    seq_t dp_next[K][K];

    /* Initialise zero counts. */
    for (size_t i = 0; i < K; ++i)
        for (size_t j = 0; j < K; ++j)
            dp_prev[i][j] = 0;

    /* First two rows – restrict the first pair by vertical pair rule. */
    for (size_t i = 0; i < K; ++i)
        for (size_t j = 0; j < K; ++j)
            if ( ((~H[i]) & H[j]) == 0 )
                dp_prev[i][j] = 1;

    idx_t rows = n + 1;          /* Total number of rows in the array. */

    /* Process rows 2 … rows-1. */
    for (idx_t r = 2; r < rows; ++r) {
        for (size_t i = 0; i < K; ++i)
            for (size_t j = 0; j < K; ++j)
                dp_next[i][j] = 0;

        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                seq_t ways = dp_prev[i][j];
                if (!ways) continue;

                for (size_t t = 0; t < K; ++t) {
                    /* Validate vertical rule for the middle row (j) using rows i and t. */
                    if ( ((~H[j]) & H[i] & H[t]) == 0 )
                        dp_next[j][t] = dp_next[j][t] + ways;
                }
            }
        }

        /* Move new counts to the previous buffer. */
        for (size_t i = 0; i < K; ++i)
            for (size_t j = 0; j < K; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* Final vertical rule for the last row (row = rows-1) using its above neighbour. */
    seq_t result = 0;
    for (size_t i = 0; i < K; ++i) {
        for (size_t j = 0; j < K; ++j) {
            seq_t ways = dp_prev[i][j];
            if (!ways) continue;
            if ( ((~H[j]) & H[i]) == 0 )
                result = result + ways;
        }
    }

    return result;
}
