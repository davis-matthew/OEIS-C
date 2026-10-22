/*
 * A000032  Lucas numbers beginning at 2: L(n) = L(n-1) + L(n-2), L(0) = 2,
 *          L(1) = 1.
 * https://oeis.org/A000032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, core, changed
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 2, y = 1, t;
    idx_t i;
    if (n == 0)
        return x;
    for (i = 1; i < n; i++) {
        t = x + y;
        x = y;
        y = t;
    }
    return y;
}
