/*
 * A008487  Expansion of (1-x^5) / (1-x)^5.
 * https://oeis.org/A008487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n == 0; otherwise a(n) = (5*n^3 + 25*n)/6
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use a 128‑bit intermediate value to avoid overflow on all builds. */
    __uint128_t tn  = (unsigned __int128)n;
    __uint128_t val = (5 * tn * tn * tn + 25 * tn) / 6;
    return (seq_t)val;
}
