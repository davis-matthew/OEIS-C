/*
 * A059555  Beatty sequence for 1 + gamma A001620.
 * https://oeis.org/A059555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * (1 + γ) ) where γ is Euler's constant ≈
 *            0.57721566490153286
 * Range:     u8 n <= 162, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use a high‑precision integer approximation of (1+γ) multiplied by 10^15.  */
#include <stdint.h>

/* 1+γ ≈ 1.57721566490153286 …  scaled by 10^15  */
static const uint64_t ALPHA_SCALED = 1577215664901533ULL;   /* round to 15 decimal places */
static const uint64_t SCALE = 1000000000000000ULL;          /* 10^15 */

/* Compute floor(n * (1+γ)) using 128‑bit arithmetic to avoid overflow   */
seq_t a(idx_t n) {
    __int128 prod = (__int128) n * (__int128) ALPHA_SCALED;
    return (seq_t)(prod / (__int128) SCALE);
}
