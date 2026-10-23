/*
 * A115790  a(n) = 1 - (floor((n+1)*Pi) - floor(n*Pi)) mod 2.
 * https://oeis.org/A115790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 1 - (floor((n+1)*Pi) - floor(n*Pi)) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of Pi:
       Pi ≈ 3141592653589793 / 10^15   (error < 1e‑16)          */
    static const uint64_t DEN   = 1000000000000000ULL;   /* 10^15            */
    static const uint64_t STEP  = 141592653589793ULL;    /* Pi*DEN mod DEN   */
    static const uint64_t THRESH= 858407346410207ULL;    /* 4*DEN - Pi*DEN   */

    unsigned __int128 prod = (unsigned __int128)n * STEP;
    uint64_t r = (uint64_t)(prod % DEN);                /* r = (n*STEP) mod DEN */

    return (seq_t)(r >= THRESH ? 1 : 0);
}
