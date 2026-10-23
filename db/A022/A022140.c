/*
 * A022140  Fibonacci sequence beginning 5, 16.
 * https://oeis.org/A022140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 5, a(1) = 16
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)16;

    seq_t f0 = 5, f1 = 16;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    return f1;
}
