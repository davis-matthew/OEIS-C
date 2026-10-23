/*
 * A047852  a(n) = A047848(4, n).
 * https://oeis.org/A047852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^n + 5)/6; recurrence a(0) = 1, a(n) = 7*a(n-1) - 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)7 - (seq_t)5;
    }
    return val;
}
