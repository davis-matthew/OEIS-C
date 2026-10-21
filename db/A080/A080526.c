/*
 * A080526  Final entry in n-th row of triangle in A080524.
 * https://oeis.org/A080526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) + (n-1)/(1+(n mod 2)) (ie. a(n) = n^(n-1) +
 *            (n-1)/2 for odd n, and a(n) = n^(n-1) + n-1 for even n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* n >= 1 by the sequence offset */
    seq_t base = (seq_t)n;
    /* compute n^(n-1) */
    seq_t power = pow_int(base, n - 1);

    /* compute the additive term */
    seq_t add;
    if (n & 1U)               /* n is odd */
        add = (seq_t)(n - 1) / 2;
    else                      /* n is even */
        add = (seq_t)n - 1;

    return power + add;
}
