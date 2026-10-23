/*
 * A040480  Continued fraction for sqrt(503).
 * https://oeis.org/A040480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 22; for n >= 1, a(n) = {2,2,1,21,1,2,2,44}[(n-1)%8]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[8] = {2, 2, 1, 21, 1, 2, 2, 44};

    if (n == 0)
        return 22;

    return period[(size_t)((n - 1) & 7)];
}
