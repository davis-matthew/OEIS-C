/*
 * A047285  Numbers that are congruent to {0, 2, 3, 6} mod 7.
 * https://oeis.org/A047285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + [0, 2, 3, 6][(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th non‑negative integer congruent to 0, 2, 3 or 6 modulo 7. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the OEIS offset. */
    idx_t q = (n - 1) / 4;     /* integer division */
    idx_t r = (n - 1) % 4;     /* 0..3 */
    static const seq_t rem[4] = {0, 2, 3, 6};
    seq_t result = (seq_t)7 * (seq_t)q + rem[r];
    return result;
}
