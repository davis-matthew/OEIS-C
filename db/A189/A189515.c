/*
 * A189515  a(n) = n + [ns] + [nt]; s=Pi/2, t=2/Pi.
 * https://oeis.org/A189515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*π/2) + floor(2n/π)
 * Range:     u8 n <= 80, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 1e18 scaling factor for rational approximations: */
static const uint64_t SCALE  = 1000000000000000000ULL;          /* 10^18 */
static const uint64_t PI_NUM = 3141592653589793238ULL;          /* π * SCALE, truncated */
static const uint64_t TWO_PI_INV_NUM = 636619772367581343ULL;   /* 2/π * SCALE, truncated */

seq_t a(idx_t n)
{
    /* Compute floor(n * π / 2) */
    __int128 prod1 = (__int128)n * PI_NUM;
    seq_t term1 = (seq_t)(prod1 / (2 * SCALE));

    /* Compute floor(2n / π) = floor(n * (2/π)) */
    __int128 prod2 = (__int128)n * TWO_PI_INV_NUM;
    seq_t term2 = (seq_t)(prod2 / SCALE);

    return (seq_t)n + term1 + term2;
}
