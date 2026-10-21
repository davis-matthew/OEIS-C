/*
 * A218303  E.g.f. A(x) satisfies A( x/(exp(2*x)*cosh(2*x)) ) =
 *          exp(x)*cosh(x).
 * https://oeis.org/A218303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{k=0..n} (2k+1)^(n-1) * binomial(n,k), with a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

static seq_t pow_odd(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t exp = n - 1;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base = (seq_t)(2 * k + 1);
        seq_t pw   = pow_odd(base, exp);      /* (2k+1)^(n-1) */
        seq_t bin  = binomial(n, k);          /* binomial(n,k)   */
        seq_t prod = seq_mul(bin, pw);        /* product term */
        sum = sum + prod;
    }

    /* result = sum / 2 */
    return sum / (seq_t)2;
}
