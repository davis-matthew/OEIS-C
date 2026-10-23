/*
 * A174438  Numbers that are congruent to {0, 2, 5, 8} mod 9.
 * https://oeis.org/A174438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * ((n-1)/4) + [0,2,5,8][(n-1)%4] (0‑based residue)
 *            Sequence: numbers congruent to {0,2,5,8} mod 9, offset=1
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t rem[4] = {0, 2, 5, 8};
    seq_t base = (seq_t)(((n - 1) / 4) * 9);
    seq_t r    = rem[(size_t)((n - 1) % 4)];
    return base + r;
}
