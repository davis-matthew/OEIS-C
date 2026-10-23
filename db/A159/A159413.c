/*
 * A159413  Number of n X n arrays of squares of integers summing to 24.
 * https://oeis.org/A159413
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n×n arrays of squares of integers summing to 24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined for n < 2 */

    idx_t N = n * n;            /* total number of cells */

    seq_t total = 0;

    /* Enumerate all integer solutions of c1 + 4c4 + 9c9 + 16c16 = 24
       where c0 = N - (c1 + c4 + c9 + c16).  All counts are non‑negative. */
    for (idx_t c16 = 0; c16 <= 1; ++c16) {                 /* 16*2 > 24 */
        idx_t rem1 = 24 - 16 * c16;                        /* remaining sum */
        idx_t max_c9 = rem1 / 9;
        for (idx_t c9 = 0; c9 <= max_c9; ++c9) {
            idx_t rem2 = rem1 - 9 * c9;                    /* remaining sum after 9s */
            idx_t max_c4 = rem2 / 4;
            for (idx_t c4 = 0; c4 <= max_c4; ++c4) {
                idx_t rem3 = rem2 - 4 * c4;                /* remaining sum after 4s */
                idx_t c1 = rem3;                           /* ones fill the rest (must be ≥0) */

                /* Number of ways to place the values:
                   choose positions for c16, then c9, then c4, then c1. */
                seq_t term = binom(N, c16);
                term *= binom(N - c16, c9);
                term *= binom(N - c16 - c9, c4);
                term *= binom(N - c16 - c9 - c4, c1);

                total += term;
            }
        }
    }

    return total;
}
