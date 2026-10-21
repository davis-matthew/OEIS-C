/*
 * A239053  Sum of divisors of 4*n-1.
 * https://oeis.org/A239053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000203(4*n-1) = sum of divisors of (4*n-1)
 * Range:     u8 n <= 42, u16 n <= 8043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1, but we guard against n==0 */
    if (n == 0) return 0;
    idx_t m = n * 4u - 1u;   /* 4*n-1 fits in idx_t for all practical n */
    return sigma_of(m);
}
