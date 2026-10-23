/*
 * A163826  G.f.: Sum_{n>=1} n * 2^(n^2) * x^n / (1 - 2^n*x)^(n+1).
 * https://oeis.org/A163826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^n * (2^n + 1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* 2^n as a seq_t */
    seq_t twon = pow2(n);

    /* (2^n + 1) as the base for the exponent */
    seq_t base = twon + 1;

    /* (2^n + 1)^(n-1) */
    seq_t power = pow_seq(base, (int)(n - 1));

    /* 2^n * (2^n + 1)^(n-1) */
    seq_t temp = twon * power;

    /* multiply by n */
    seq_t result = ((seq_t)n) * temp;

    return result;
}
