/*
 * A176900  a(n) = sin((2*n+5)*Pi/6)*(n+1)*2^(n+1).
 * https://oeis.org/A176900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sin((2*n+5)*Pi/6)*(n+1)*2^(n+1)
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 25, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sign and factor adjustments for n mod 6 */
    static const int sign_table[6] = { 1, -1, -1, -1,  1,  1 };
    static const int extra_table[6] = { 0,  0,  1,  0,  0,  1 };

    idx_t r = n % 6;
    seq_t val = (seq_t)n + 1;          /* (n+1) */
    idx_t exp = n + extra_table[r];    /* exponent for power of two */

    val *= pow2(exp);                  /* (n+1) * 2^exp */

    if (sign_table[r] == -1) val = -val;

    return val;
}
