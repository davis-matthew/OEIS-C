/*
 * A016175  Expansion of 1/((1-6*x)*(1-12*x)).
 * https://oeis.org/A016175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*12^n - 6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 12^n and 6^n via binary exponentiation */
    seq_t pow12n = seq_pow(12, n);
    seq_t pow6n  = seq_pow(6, n);

    /* a(n) = 2*12^n - 6^n */
    seq_t result = pow12n;
    result += pow12n;   /* 2*12^n */
    result -= pow6n;    /* subtract 6^n */
    return result;
}
