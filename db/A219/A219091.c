/*
 * A219091  a(n) = floor((n + 1/2)^8).
 * https://oeis.org/A219091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = floor((n + 1/2)^8) for any n >= 0.
   Uses 128‑bit arithmetic when available to
   avoid overflow before the final division by 256. */
seq_t a(idx_t n)
{
    /* (2*n + 1)^8 / 256  */
    /* Compute (2*n + 1) in a 128‑bit variable. */
    unsigned __int128 x = ((unsigned __int128)2) * n + 1;

    /* Raise x to the 8th power. */
    unsigned __int128 p = x;
    for (int i = 0; i < 7; ++i)
        p *= x;        /* p = x^8 after the loop */

    /* shift right by 8 bits (divide by 256) and cast back to seq_t */
    return (seq_t)(p >> 8);
}
