/*
 * A000330  Square pyramidal numbers: a(n) = 0^2 + 1^2 + 2^2 + ... + n^2 =
 *          n*(n+1)*(2*n+1)/6.
 * https://oeis.org/A000330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = n*(n+1)*(2*n+1)/6
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 2343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t x = n, y = n + 1, z;
    idx_t r = n % 3;

    if (r == 0) {
        x /= 3;
        z = 2 * n + 1;
    } else if (r == 2) {
        y /= 3;
        z = 2 * n + 1;
    } else {
        /* n = 3k+1, so 2n+1 = 6k+3 and (2n+1)/3 = 2k+1 */
        z = 2 * (n / 3) + 1;
    }

    /* dividing by 3 keeps parity, so exactly one of x, y is still even */
    if ((n & 1) == 0)
        x /= 2;
    else
        y /= 2;

    return (seq_t)x * (seq_t)y * (seq_t)z;
}
