/*
 * A201824  G.f.: Sum_{n>=0} log( 1/sqrt(1-2^n*x) )^n / n!.
 * https://oeis.org/A201824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^(n-1) + n - 1, n) for n>0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* For n >= 1 compute binomial(2^(n-1) + n - 1, n) */
    seq_t pow = pow2(n - 1);        /* 2^(n-1) */
    seq_t numerator = pow + (seq_t)(n - 1);  /* 2^(n-1) + n - 1 */

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = numerator - (seq_t)(n - i); /* current multiplicand */
        result = result * term;
        result = result / (seq_t)i;             /* exact division */
    }
    return result;
}
