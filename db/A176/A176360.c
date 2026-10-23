/*
 * A176360  a(n) = quadrant of unit circle corresponding to n radians.
 * https://oeis.org/A176360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (floor(2*n/Pi) mod 4)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^18, used to keep Pi as a rational numerator/denominator pair.
   PI_NUM = floor(pi * 10^18).  */
static const uint64_t DEN = 1000000000000000000ULL;          /* 10^18   */
static const uint64_t PI_NUM = 3141592653589793238ULL;       /* floor(pi*10^18) */

/* Returns the quadrant containing the point on the unit circle
   at angle n radians.  */
seq_t a(idx_t n)
{
    /* Compute 2*n*DEN as a 128-bit integer to avoid overflow. */
    unsigned __int128 num = (unsigned __int128)n * 2u * DEN;

    /* Exact floor division by PI_NUM. */
    uint64_t q = (uint64_t)(num / PI_NUM);

    /* Quadrant is (floor(2*n/Pi) mod 4) + 1. */
    return (seq_t)(q % 4 + 1);
}
