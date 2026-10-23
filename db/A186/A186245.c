/*
 * A186245  a(n) = binomial(n^2, 2*n).
 * https://oeis.org/A186245
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n^2, 2*n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset, the value is irrelevant. */
    if (n < 2) return 0;
    return binom(n * n, 2 * n);
}
