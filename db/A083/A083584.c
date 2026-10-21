/*
 * A083584  a(n) = (8*4^n - 5)/3.
 * https://oeis.org/A083584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8*4^n - 5)/3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 4*a(n-1) + 5 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = 4 * val + 5;
    }
    return val;
}
