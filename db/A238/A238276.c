/*
 * A238276  a(n) = (9*8^n - 2)/7.
 * https://oeis.org/A238276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*8^n - 2)/7 Recurrence: a(0) = 1; a(n) = 8*a(n-1) + 2
 *            for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;          // a(0)
    for (idx_t i = 1; i <= n; ++i) // iterate to n
    {
        res = res * (seq_t)8 + (seq_t)2;
    }
    return res;
}
