/*
 * A065599  If n odd, a(n) = n^2 else a(n) = n.
 * https://oeis.org/A065599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 if n is odd, a(n) = n otherwise
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n & 1U) {
        seq_t v = (seq_t)n;
        return v * v;
    } else {
        return (seq_t)n;
    }
}
