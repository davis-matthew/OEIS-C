/*
 * A229338  Product of products of elements of subsets of divisors of n.
 * https://oeis.org/A229338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(tau(n)*2^(tau(n)-2)) where tau(n) = A000005(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, divisor_count_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* special case: a(1) = 1 */

    idx_t tau = divisor_count_u64(n);   /* number of divisors of n */

    /* exponent e = tau * 2^(tau-2) */
    seq_t e = pow2(tau - 2) * (seq_t)tau;

    seq_t base = (seq_t)n;
    return big_pow(base, e);
}
