/*
 * A002994  Initial digit of cubes.
 * https://oeis.org/A002994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A000030(n^3): leading decimal digit of n^3, a(0) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned __int128 c = (unsigned __int128)n * n * n;
    while (c >= 10)
        c /= 10;
    return (seq_t)(unsigned)c;
}
