/*
 * A199565  a(n) = (7*9^n + 1)/4.
 * https://oeis.org/A199565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*9^n + 1)/4 Recurrence: a(0) = 2; a(n) = 9*a(n-1) - 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 2;
    for (idx_t i = 1; i <= n; ++i)
        res = 9 * res - 2;
    return res;
}
