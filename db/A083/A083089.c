/*
 * A083089  Numbers that are congruent to {0, 2, 4, 6, 7, 9, 11} mod 12.
 * https://oeis.org/A083089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*floor((n-1)/7) + {0,2,4,6,7,9,11}[(n-1)%7]
 * Range:     u8 n <= 149, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // n is at least 1, behavior for n==0 is irrelevant
    static const uint8_t offsets[7] = {0, 2, 4, 6, 7, 9, 11};

    idx_t q = (n - 1) / 7;          // block number (0-based)
    idx_t r = (n - 1) % 7;          // position inside the block (0..6)

    seq_t base = (seq_t)q * (seq_t)12; // 12 per full block
    return base + (seq_t)offsets[r];
}
