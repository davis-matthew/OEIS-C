/*
 * A116419  Reduced numerators of 2*(2^(1+n)-1)/(1+n)/(2+n).
 * https://oeis.org/A116419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of reduced fraction of
 *            2*(2^(n+1)-1)/((n+1)*(n+2))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 1));

    /* Numerator N = 2*(2^(n+1)-1) */
    seq_t N = (pow2 - (seq_t)1) * (seq_t)2;

    /* Denominator D = (n+1)*(n+2) */
    seq_t D = (seq_t)(n + 1) * (seq_t)(n + 2);

    /* Reduce the fraction */
    seq_t g = gcd(N, D);

    return N / g;
}
