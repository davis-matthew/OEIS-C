/*
 * A055318  Number of labeled trees with n nodes and 6 leaves.
 * https://oeis.org/A055318
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(15*n^3 - 240*n^2 +
 *            1265*n - 2192) / 4147200 (i.e. a(n) = (n! / 6!) * Stirling2(n-2,
 *            n-6) ) This implementation cancels the constant denominator
 *            4147200 = 2^11 * 3^4 * 5^2 while multiplying the numerator terms
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for the standard integer types */

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 7.  For smaller n
       return 0 (the behaviour is irrelevant). */
    if (n < 7) return 0;

    /* Remaining powers of the denominator that still need to be
       cancelled.  We cancel them on the fly from the factors we
       multiply. */
    int rem2 = 11;   /* 2^11 */
    int rem3 = 4;    /* 3^4  */
    int rem5 = 2;    /* 5^2  */

    seq_t result = 1;

    /* Multiply factorial n! while cancelling denominator factors. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t f = (seq_t)i;
        /* cancel powers of 2 from f while we still need them */
        while (rem2 > 0 && (f % 2) == 0)
        { f /= 2; --rem2; }
        while (rem3 > 0 && (f % 3) == 0)
        { f /= 3; --rem3; }
        while (rem5 > 0 && (f % 5) == 0)
        { f /= 5; --rem5; }
        result *= f;
    }

    /* Multiply the five factors (n-6) … (n-2). */
    for (int t = 6; t >= 2; --t)
    {
        seq_t f = (seq_t)(n - t);
        while (rem2 > 0 && (f % 2) == 0)
        { f /= 2; --rem2; }
        while (rem3 > 0 && (f % 3) == 0)
        { f /= 3; --rem3; }
        while (rem5 > 0 && (f % 5) == 0)
        { f /= 5; --rem5; }
        result *= f;
    }

    /* Compute the cubic polynomial factor and multiply it in. */
    seq_t nn = (seq_t)n;
    seq_t poly = (seq_t)15 * nn * nn * nn;
    poly -= (seq_t)240 * nn * nn;
    poly += (seq_t)1265 * nn;
    poly -= (seq_t)2192;

    while (rem2 > 0 && (poly % 2) == 0)
    { poly /= 2; --rem2; }
    while (rem3 > 0 && (poly % 3) == 0)
    { poly /= 3; --rem3; }
    while (rem5 > 0 && (poly % 5) == 0)
    { poly /= 5; --rem5; }
    result *= poly;

    /* If any prime powers are left, divide them out now. */
    if (rem2 > 0)
    {
        seq_t pow2 = 1;
        for (int i = 0; i < rem2; ++i) pow2 *= (seq_t)2;
        result /= pow2;
    }
    if (rem3 > 0)
    {
        seq_t pow3 = 1;
        for (int i = 0; i < rem3; ++i) pow3 *= (seq_t)3;
        result /= pow3;
    }
    if (rem5 > 0)
    {
        seq_t pow5 = 1;
        for (int i = 0; i < rem5; ++i) pow5 *= (seq_t)5;
        result /= pow5;
    }

    return result;
}
