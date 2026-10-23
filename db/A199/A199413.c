/*
 * A199413  a(n) = 9*6^n+1.
 * https://oeis.org/A199413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*6^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 10; a(n) = 6*a(n-1) - 5 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)10;

    seq_t val = (seq_t)10;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        val = (seq_t)6 * val - (seq_t)5;

    return val;
}
