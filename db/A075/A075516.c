/*
 * A075516  Sixth column of triangle A075498.
 * https://oeis.org/A075516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * S2(n+6, 6) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Stirling numbers S(i, k) for k = 0..6, updated iteratively. */
    seq_t stir[7];
    for (int k = 0; k < 7; ++k) stir[k] = 0;
    stir[0] = 1;                /* S(0,0) = 1 */

    idx_t limit = n + 6;         /* we need S(n+6, 6) */

    for (idx_t i = 1; i <= limit; ++i) {
        /* Update from high k to low to avoid overwriting needed values. */
        for (int k = 6; k >= 1; --k) {
            /* S(i, k) = k * S(i-1, k) + S(i-1, k-1) */
            stir[k] = stir[k] * (seq_t)k + stir[k - 1];
        }
        stir[0] = 0;              /* S(i, 0) = 0 for i > 0 */
    }

    /* compute 3^n */
    seq_t pow3n = pow3(n);

    /* a(n) = 3^n * S(n+6, 6) */
    return pow3n * stir[6];
}
