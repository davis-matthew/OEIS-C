/*
 * A024499  a(n) = [ C(2n,n)/(n-1) ].
 * https://oeis.org/A024499
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n)/(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* term undefined for n < 2 */
    /* compute binomial coefficient C(2n,n) */
    seq_t bin = binom((idx_t)(2 * n), n);
    /* divide by (n-1) */
    return bin / ((seq_t)n - 1);
}
