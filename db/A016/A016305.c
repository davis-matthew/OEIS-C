/*
 * A016305  Expansion of 1/((1-2*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A016305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^(2*n+5) - 3^(n+3) + 1)/3; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    /* Compute the inner expression */
    idx_t e1 = 2 * n + 5;                 /* exponent for 2^(2*n+5)  */
    idx_t e2 = n + 3;                     /* exponent for 3^(n+3)    */
    seq_t term = pow2(e1);                /* 2^(2*n+5)              */
    term -= pow3(e2);                    /* 2^(2*n+5) - 3^(n+3)    */
    term += (seq_t)1;                    /* +1                       */
    term /= (seq_t)3;                    /* divide by 3 (guaranteed integer result) */

    /* Multiply by 2^(n-1) */
    seq_t mult = pow2(n - 1);             /* 2^(n-1)                */
    return term * mult;                   /* final a(n)              */
}
