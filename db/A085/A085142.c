/*
 * A085142  Duplicate of A064053.
 * https://oeis.org/A085142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1; for n >= 1, a(n) = Sum_{k>=1, k | (n - k(3k+1)/2),
 *            m=(n - k(3k+1)/2)/k >= 0} 4*(-1)^(k+m) (duplicate of A064053)
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A064053_a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 0;
    for (idx_t k = 1; ; ++k)
    {
        idx_t exponent = k * (3 * k + 1) / 2;
        if (exponent > n)
            break;
        idx_t diff = n - exponent;
        if (diff % k != 0)
            continue;
        idx_t m = diff / k;
        if ((k + m) & 1)
            result -= 4;
        else
            result += 4;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A064053_a(n);
}
