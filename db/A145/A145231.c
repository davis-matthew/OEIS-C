/*
 * A145231  a(n) = Fibonacci(4^n).
 * https://oeis.org/A145231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4^n) Recurrence from Peter Bala: a(0) = 1 a(1)
 *            = 3 a(n+1) = a(n) * (5 * a(n)^2 + 2) * sqrt(5 * a(n)^2 + 4) (n ≥
 *            1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"  /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* a(1) is known */
    seq_t val = (seq_t)3;

    if (n == 1)
        return val;

    for (idx_t i = 1; i < n; ++i) {
        seq_t sq  = val * val;                     /* val^2      */
        seq_t five_s = 5 * sq + 2;                 /* 5*val^2+2  */
        seq_t under  = 5 * sq + 4;                 /* 5*val^2+4  */
        seq_t sqrt_under = floor_sqrt(under);      /* sqrt(…)    */

        val = val * five_s;
        val = val * sqrt_under;
    }

    return val;
}
