/*
 * A053566  Expansion of (11*x-2)/(1-3*x)^2.
 * https://oeis.org/A053566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3^(n-1)*(5*n-6) for n≥1, a(0) = -2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 36, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)(-2);

    /* compute multiplier (5*n - 6) as signed to handle small n */
    int64_t mult = 5 * (int64_t)n - 6;
    seq_t pow3 = (seq_t)1;
    /* 3^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow3 *= (seq_t)3;

    return pow3 * (seq_t)mult;
}
