/*
 * A000308  a(n) = a(n-1)*a(n-2)*a(n-3) with a(1)=1, a(2)=2 and a(3)=3.
 * https://oeis.org/A000308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3), a(1) = 1, a(2) = 2, a(3) = 3
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x, y, z, t;
    idx_t i;

    if (n <= 3)
        return (seq_t)n;

    x = (seq_t)1;  /* a(i-3) */
    y = (seq_t)2;  /* a(i-2) */
    z = (seq_t)3;  /* a(i-1) */
    for (i = 4; i <= n; i++) {
        t = z * y * x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
