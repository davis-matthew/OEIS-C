/*
 * A000225  a(n) = 2^n - 1. (Sometimes called Mersenne numbers, although that
 *          name is usually reserved for A001348.)
 * https://oeis.org/A000225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice, changed
 * Formula:   a(n) = 2*a(n-1) + 1, a(0) = 0 (i.e. a(n) = 2^n - 1)
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
    for (i = 0; i < n; i++) {
        r = r * 2 + 1;
    }
    return r;
}
