/*
 * A206178  a(n) = Sum_{k=0..n} binomial(n,k)^3 * 2^k.
 * https://oeis.org/A206178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * 2^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t pow2 = 1;   /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t b    = binomial(n, k);   /* C(n,k) */
        seq_t term = b * b * b;        /* C(n,k)^3 */
        term = term * pow2;            /* * 2^k */
        sum = sum + term;

        if (k < n)                     /* prepare 2^(k+1) for the next iteration */
            pow2 = pow2 + pow2;         /* multiply by 2 via addition */
    }
    return sum;
}
