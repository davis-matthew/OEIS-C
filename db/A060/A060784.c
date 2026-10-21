/*
 * A060784  Number of double tangents of order n.
 * https://oeis.org/A060784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-2)*(n-3)*(n+3)/2
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 128‑bit to avoid overflow and correctly handle negative
       intermediate results for n < 2. */
    signed __int128 x = (signed __int128)n;
    signed __int128 prod = x * (x - 2) * (x - 3) * (x + 3);
    prod /= 2;
    return (seq_t)prod;
}
