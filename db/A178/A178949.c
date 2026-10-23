/*
 * A178949  E.g.f. satisfies: A(x) = exp(x^2*A(x)) where A(x) = Sum_{n>=0}
 *          a(n)*x^(2n)/(2n)!.
 * https://oeis.org/A178949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n-1) * (2n)! / n! for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"   /* mul_small(seq_t, unsigned int) */
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t, idx_t) */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute prod = Π_{k=n+1}^{2n} k = (2n)! / n! */
    seq_t prod = (seq_t)1;
    for (idx_t k = n + 1; k <= (idx_t)(2 * n); ++k) {
        prod = mul_small(prod, (unsigned int)k);
    }

    /* Compute pw = (n+1)^(n-1) */
    seq_t pw = pow_uint((uint64_t)(n + 1), n - 1);

    /* Final result = prod * pw */
    return prod * pw;
}
