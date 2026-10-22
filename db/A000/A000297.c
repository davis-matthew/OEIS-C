/*
 * A000297  a(n) = (n+1)*(n+3)*(n+8)/6.
 * https://oeis.org/A000297
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+3)*(n+8)/6
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t x = n + 1, y = n + 3, z = n + 8;
    if (x < 0) return (seq_t)0;
    /* divide out 2 and 3 before multiplying to keep intermediates small */
    if (x % 2 == 0) x /= 2; else z /= 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    return (seq_t)x * (seq_t)y * (seq_t)z;
}
