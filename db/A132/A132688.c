/*
 * A132688  a(n) = binomial(2^n + 3*n, n).
 * https://oeis.org/A132688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + 3*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* upper parameter N = 2^n + 3*n */
    seq_t N = pow2(n);            /* 2^n  */
    N += (seq_t)3 * (seq_t)n;     /* + 3*n */

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t numerator = N - (seq_t)(i - 1);  /* N - (i-1) */
        seq_t denom     = (seq_t)i;           /* i */

        /* cancel common factors with the current result */
        seq_t g = gcd(result, denom);
        result /= g;
        denom   /= g;

        /* cancel common factors between numerator and remaining denom */
        g = gcd(numerator, denom);
        numerator /= g;
        denom     /= g;   /* denom should be 1 now */

        result *= numerator;
    }
    return result;
}
