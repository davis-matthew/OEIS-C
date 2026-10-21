/*
 * A224692  Expansion of (1+5*x+7*x^2-x^3)/((1-2*x^2)*(1-x)*(1+x)).
 * https://oeis.org/A224692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^⌊n/2⌋ - 2*(-1)^n - 6 (equivalently: a(2n) = 9*2^n -
 *            8, a(2n+1) = 9*2^n - 4)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t val  = pow2(half);      /* 2^half */
    val *= (seq_t)9;              /* 9*2^half */

    if ((n & 1) == 0) {           /* even n -> subtract 8 */
        val -= (seq_t)8;
    } else {                      /* odd n  -> subtract 4 */
        val -= (seq_t)4;
    }
    return val;
}
