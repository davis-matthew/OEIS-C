/*
 * A032093  Number of reversible strings with n-1 beads of 2 colors. 6 beads
 *          are black. Strings are not palindromic.
 * https://oeis.org/A032093
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(binomial(n-1, n-7) - binomial(floor((n-1)/2),
 *            floor((n-7)/2)))
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 8) {
        return 0;
    }

    /* binomial(n-1, n-7) == binomial(n-1, 6) */
    seq_t first  = binom(n - 1, (idx_t)6);

    /* binomial(floor((n-1)/2), floor((n-7)/2)) */
    idx_t half_n_minus_1 = (n - 1) / 2;
    idx_t half_n_minus_7 = (n - 7) / 2;
    seq_t second = binom(half_n_minus_1, half_n_minus_7);

    seq_t diff = first - second;
    return diff / 2;
}
