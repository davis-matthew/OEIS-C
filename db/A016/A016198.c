/*
 * A016198  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-5*x)).
 * https://oeis.org/A016198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*5^n - 16*2^n + 3)/12
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute a(n) = (25*5^n - 16*2^n + 3)/12 */
    seq_t five_pow = power5(n);          /* 5^n */
    five_pow = five_pow * (seq_t)25;      /* 25*5^n */

    seq_t two_pow = pow2(n);             /* 2^n */
    two_pow = two_pow * (seq_t)16;        /* 16*2^n */

    seq_t num = five_pow - two_pow + (seq_t)3; /* numerator */
    return num / (seq_t)12;              /* division by 12 */
}
