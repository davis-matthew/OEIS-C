/*
 * A178759  Expansion of e.g.f. 3*x*exp(x)*(exp(x)-1)^2.
 * https://oeis.org/A178759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 3*2^n + 3)*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 3^n and 2^n */
    seq_t pow3 = big_pow(3, (seq_t)n);
    seq_t pow2 = big_pow(2, (seq_t)n);

    /* use (3^n + 3 - 3*2^n) to avoid unsigned underflow */
    seq_t result = pow3;
    result += 3;                 /* add the constant 3 */
    result -= 3 * pow2;          /* subtract 3*2^n */

    /* multiply by n (converted to seq_t) */
    result *= (seq_t)n;

    return result;
}
