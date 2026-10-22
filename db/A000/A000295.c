/*
 * A000295  Eulerian numbers (Euler's triangle: column k=2 of A008292, column
 *          k=1 of A173018).
 * https://oeis.org/A000295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*a(n-1) + n - 1, a(0) = 0 (equivalently a(n) = 2^n - n -
 *            1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        r = r * 2 + (seq_t)(i - 1);
    }
    return r;
}
