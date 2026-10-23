/*
 * A047532  Numbers that are congruent to {0, 2, 3, 7} mod 8.
 * https://oeis.org/A047532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,2,3,7][(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t offsets[4] = {0, 2, 3, 7};

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for the sequence but safe */

    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    return (seq_t)q * 8 + offsets[r];
}
