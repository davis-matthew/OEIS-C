/*
 * A240883  Central terms of the triangle in A240857.
 * https://oeis.org/A240883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046818(n) - 1 = (number of 1's in binary expansion of
 *            3n+1) - 1
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9988 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 3 + 1;
    seq_t count = 0;
    while (x) {
        if (x & 1) {
            count = count + 1;
        }
        x = x >> 1;
    }
    return count - 1;
}
