/*
 * A079044  Numbers k such that Sum_{j=0..k} sin(j/Pi) < 0.
 * https://oeis.org/A079044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2*π² * n ) (conjectured and verified for n≤10⁴)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1, value for 0 is unused */

    /* 2*π² ≈ 19.739208802178717237…  */
    /* We use 16‑digit scaling:    C = floor(2*π² * 10¹⁶) */
    static const uint64_t C = 197392088021787172ULL;   /* 2*π² * 10¹⁶  */
    static const uint64_t D = 10000000000000000ULL;    /* 10¹⁶          */

    __uint128_t prod = ( __uint128_t)n * C;           /* up to 128‑bit */
    seq_t result = (seq_t)(prod / D);                  /* floor division */

    return result;
}
