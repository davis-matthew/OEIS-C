/*
 * A154410  a(n) = 5*(3*6^n + 2^n)/2.
 * https://oeis.org/A154410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(3*6^n + 2^n)/2 Recurrence: a(0) = 10, a(1) = 50, a(n)
 *            = 8*a(n-1) - 12*a(n-2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)50;

    seq_t prev2 = (seq_t)10;
    seq_t prev1 = (seq_t)50;
    seq_t cur, i;

    for (i = 2; i <= n; ++i) {
        cur = 8 * prev1 - 12 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
