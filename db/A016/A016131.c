/*
 * A016131  Expansion of 1/((1-2*x)*(1-8*x)).
 * https://oeis.org/A016131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(3*n+2) - 2^n) / 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(2*n+2)  ->  2^(3*n+2) = 2^(2*n+2) * 2^n
       Use the fact that (2^(2*n+2) - 1) is divisible by 3 */
    idx_t exp = n * 2 + 2;          /* 2*n + 2 */
    seq_t pow_large = pow2(exp);    /* 2^(2*n+2) */
    seq_t s = (pow_large - 1) / 3;  /* (2^(2*n+2) - 1)/3  */
    return pow2(n) * s;             /* 2^n * s = a(n) */
}
