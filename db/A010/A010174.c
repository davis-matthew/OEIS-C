/*
 * A010174  Continued fraction for sqrt(108).
 * https://oeis.org/A010174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 10; for n >= 1, a(n) follows the period 2, 1, 1, 4, 1, 1,
 *            2, 20, repeated cyclically
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[8] = { 2, 1, 1, 4, 1, 1, 2, 20 };

    if (n == 0)
        return (seq_t)10;

    idx_t idx = (n - 1) % (idx_t)8;
    return period[(size_t)idx];
}
