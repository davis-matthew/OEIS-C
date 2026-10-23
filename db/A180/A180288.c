/*
 * A180288  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to 8.
 * https://oeis.org/A180288
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x1+...+xn = n with 0 ≤ xi ≤ 8 and
 *            at least one xi = 8; computed as a(n) = [x^n]((1 + x + … +
 *            x^8)^n) – [x^n]((1 + x + … + x^7)^n)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 36, u128 n <= 67,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;                 /* offset is 8, terms below are irrelevant */

    /* Count solutions with all xi ≤ 8 using inclusion–exclusion */
    seq_t even8 = 0, odd8 = 0;
    idx_t maxk8 = n / 9;              /* largest k with 9k ≤ n */
    for (idx_t k = 0; k <= maxk8; ++k) {
        seq_t term = binom(n, k);
        term *= binom(2 * n - 9 * k - 1, n - 1);
        if (k & 1)
            odd8 += term;
        else
            even8 += term;
    }
    seq_t total8 = even8 - odd8;

    /* Count solutions with all xi ≤ 7 */
    seq_t even7 = 0, odd7 = 0;
    idx_t maxk7 = n / 8;              /* largest k with 8k ≤ n */
    for (idx_t k = 0; k <= maxk7; ++k) {
        seq_t term = binom(n, k);
        term *= binom(2 * n - 8 * k - 1, n - 1);
        if (k & 1)
            odd7 += term;
        else
            even7 += term;
    }
    seq_t total7 = even7 - odd7;

    /* Desired count: at least one box contains 8 balls */
    return total8 - total7;
}
