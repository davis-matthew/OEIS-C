/*
 * A202944  G.f.: Sum_{n>=0} (n+1)*(n+2)/2 * 2^(n*(n-1)) * x^n.
 * https://oeis.org/A202944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(n+2)/2 * 2^(n*(n-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* compute binomial coefficient (n+1 choose 2) */
    seq_t tri = (((seq_t)n) + 1) * (((seq_t)n) + 2) / 2;

    /* exponent for the power of two: n*(n-1) */
    idx_t exp = (idx_t)n * (n - 1);

    /* compute 2^exp */
    seq_t pow2 = pow_seq((seq_t)2, exp);

    /* final result */
    return tri * pow2;
}
