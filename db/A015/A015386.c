/*
 * A015386  Gaussian binomial coefficient [ n,10 ] for q=-2.
 * https://oeis.org/A015386
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..10} ((-2)^(n-i+1)-1)/((-2)^i-1)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 13, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation based on absolute values of numerator and denominator
   factors, cancelling common factors using gcd before multiplication.
   Requires the utilities gcd.h and pow2.h. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 10)  /* sequence is defined for n >= 10 */
        return (seq_t)0;

    const int K = 10;
    seq_t num[K];
    seq_t den[K];

    /* Precompute numerator and denominator absolute values */
    for (int i = 1; i <= K; ++i)
    {
        /* Numerator exponent: n - i + 1 */
        idx_t exp_num = n - (idx_t)i + 1;
        seq_t pow_num = pow2(exp_num);          /* 2^exp_num */

        if (exp_num & 1)        /* odd exponent: (-2)^k = -(2^k) */
            num[i - 1] = pow_num + (seq_t)1;   /* |(-2)^k - 1| = 2^k + 1 */
        else                    /* even exponent */
            num[i - 1] = pow_num - (seq_t)1;   /* |(-2)^k - 1| = 2^k - 1 */

        /* Denominator exponent: i */
        idx_t exp_den = (idx_t)i;
        seq_t pow_den = pow2(exp_den);         /* 2^i */

        if (exp_den & 1)        /* odd denominator: (-2)^i = -(2^i) */
            den[i - 1] = pow_den + (seq_t)1;   /* |(-2)^i - 1| = 2^i + 1 */
        else                    /* even denominator */
            den[i - 1] = pow_den - (seq_t)1;   /* |(-2)^i - 1| = 2^i - 1 */
    }

    /* Cancel common factors between numerators and denominators */
    for (int j = 0; j < K; ++j)
    {
        if (num[j] == (seq_t)0) continue;   /* safety, though not expected */
        for (int k = 0; k < K; ++k)
        {
            seq_t g = gcd(num[j], den[k]);
            if (g > (seq_t)1)
            {
                num[j] /= g;
                den[k] /= g;
            }
        }
    }

    /* Multiply remaining numerators */
    seq_t result = (seq_t)1;
    for (int j = 0; j < K; ++j)
        result *= num[j];

    /* Divide by remaining denominators (should all be 1 after cancellation) */
    for (int k = 0; k < K; ++k)
        if (den[k] > (seq_t)1)
            result /= den[k];

    return result;
}
