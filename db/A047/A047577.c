/*
 * A047577  Numbers that are congruent to {0, 1, 5, 6, 7} mod 8.
 * https://oeis.org/A047577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + {0,1,5,6,7}[(n-1) mod 5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This directly computes the nth number that is congruent to 0,1,5,6,or 7 (mod 8). */

#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* For n >= 1, index into the 5 allowed residues per block of 8. */
    if (n == 0) return 0;            /* behaviour for n < 1 is irrelevant */

    idx_t k = (n - 1) / 5;            /* block number (0‑based) */
    idx_t r = (n - 1) % 5;            /* position within the block */

    static const seq_t offsets[5] = {0, 1, 5, 6, 7};

    seq_t res = (seq_t)k * 8 + offsets[r];
    return res;
}
