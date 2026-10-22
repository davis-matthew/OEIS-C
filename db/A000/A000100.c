/*
 * A000100  a(n) is the number of compositions of n in which the maximal part
 *          is 3.
 * https://oeis.org/A000100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C3(n) - F(n+1), where C3(n) = C3(n-1) + C3(n-2) + C3(n-3)
 *            (compositions of n into parts <= 3), C3(0) = 1, C3(-1) = C3(-2)
 *            = 0, and F(n+1) = A000045(n+1) counts compositions of n into
 *            parts <= 2
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 1, y = 0, z = 0;   /* C3(i), C3(i-1), C3(i-2) */
    seq_t p = 1, q = 0;          /* F(i+1), F(i) */
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y + z;
        z = y;
        y = x;
        x = t;
        seq_t s = p + q;
        q = p;
        p = s;
    }
    return x - p;
}
