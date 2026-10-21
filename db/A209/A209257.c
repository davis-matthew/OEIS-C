/*
 * A209257  A musically inspired Titius-Bode-like sequence based on the
 *          geometric division of 4- and 5-dimensional space: Z_(n+1) = 3 *
 *          (C(n-1, 0) + C(n-1, 1) + C(n-1, 2) + C(n-1, 3) + C(n-1, 4) +
 *          C(n-1, 5)*A059620(n+6)) + 4.
 * https://oeis.org/A209257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (C(n-1,0)+C(n-1,1)+C(n-1,2)+C(n-1,3)+C(n-1,4) +
 *            C(n-1,5) * ( floor((5*(n+6)+7)/12) - floor((5*(n+6)+2)/12) ) ) +
 *            4 with a(0) = 4
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)4;
    }

    /* Compute sum_{k=0..4} C(n-1,k) and C(n-1,5) */
    idx_t n_minus_1 = n - 1u;
    seq_t sum = 0;
    for (idx_t k = 0; k <= 4; ++k) {
        sum += binom(n_minus_1, k);
    }
    seq_t c5 = binom(n_minus_1, 5u);

    /* Diff = floor((5*(n+6)+7)/12) - floor((5*(n+6)+2)/12)  (0 or 1) */
    idx_t t = n + 6u;                     /* n + 6 */
    idx_t a1 = (5 * t + 7u) / 12u;
    idx_t a2 = (5 * t + 2u) / 12u;
    idx_t diff = a1 - a2;                 /* diff is 0 or 1 */

    /* Final computation */
    seq_t term = sum + c5 * (seq_t)diff;
    return (seq_t)3 * term + (seq_t)4;
}
