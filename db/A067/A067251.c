/*
 * A067251  Numbers with no trailing zeros in decimal representation.
 * https://oeis.org/A067251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + floor((n-1)/9)
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute a(n) according to a(n) = n + floor((n‑1)/9) */
    return (seq_t)n + (seq_t)((n - 1) / 9);
}
