/*
 * A187735  G.f.: Sum_{n>=0} (2*n+1)^n * x^n / (1 + (2*n+1)*x)^n.
 * https://oeis.org/A187735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2) * 2^(n-1) * n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact = seq_mul(fact, (seq_t)i);

    /* Compute 2^(n-1) */
    seq_t p2 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        p2 = seq_mul(p2, (seq_t)2);

    /* a(n) = (n+2) * 2^(n-1) * n! */
    seq_t result = seq_mul(fact, p2);
    result = seq_mul(result, (seq_t)(n + 2));
    return result;
}
