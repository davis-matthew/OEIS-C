/*
 * A180491  Product of remainders of n mod k, for k = 2,3,4,...,n-1.
 * https://oeis.org/A180491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 22, u64 n <= 36, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS A180491: the product of remainders n mod k for
 * k = 2,3,...,n-1.  If n is composite the product contains a factor
 * n % d = 0 for some divisor d, hence the result is zero.  The
 * function uses the provided is_prime utility. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Special cases that match the OEIS offset */
    if (n <= 3)
        return (seq_t)1;

    /* Zero for any composite n */
    if (!is_prime(n))
        return (seq_t)0;

    /* n is prime; compute the product of remainders */
    seq_t prod = (seq_t)1;
    for (idx_t k = 2; k < n; ++k)
        prod *= (seq_t)(n % k);
    return prod;
}
