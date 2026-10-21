/*
 * A235361  Floor((n + Pi)^2).
 * https://oeis.org/A235361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + pi)^2)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define SCALE 1000000000000ULL       /* 10^12, scaling factor for π */
#define PI_SCALED 3141592653589ULL   /* floor(π * SCALE) */

/* Computes a(n) = floor((n + π)^2) for n >= 0.
   Uses integer arithmetic only. */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate products to avoid overflow. */
    unsigned __int128 x   = (unsigned __int128)n * SCALE + PI_SCALED;
    unsigned __int128 num = x * x;
    unsigned __int128 den = (unsigned __int128)SCALE * SCALE;
    unsigned __int128 res = num / den;
    return (seq_t)res;
}
