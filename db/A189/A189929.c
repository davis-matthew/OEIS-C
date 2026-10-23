/*
 * A189929  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sin(2pi/5), t=cos(2pi/5).
 * https://oeis.org/A189929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sin(2π/5)) + floor(n*cos(2π/5))
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer approximations for the irrational constants:
 *  sin(2π/5) ≈ 0.9510565163  ≈ 951056516 / 1000000000
 *  cos(2π/5) ≈ 0.3090169944  ≈ 309016994 / 1000000000
 *  The denominator 1e9 guarantees that for n ≤ 10⁶ the error is < 1/n,
 *  so floor(n*value) is computed exactly with integer arithmetic. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Numerators and denominator for the two irrational constants. */
    const uint64_t NUM_S = 951056516ULL;
    const uint64_t DEN_S = 1000000000ULL;
    const uint64_t NUM_T = 309016994ULL;
    const uint64_t DEN_T = 1000000000ULL;

    /* Compute floor(n * sin(2π/5)) and floor(n * cos(2π/5)) using 128-bit intermediates
     * to avoid overflow for large n. */
    seq_t floor_s = ( (unsigned __int128)n * NUM_S ) / DEN_S;
    seq_t floor_t = ( (unsigned __int128)n * NUM_T ) / DEN_T;

    /* Return the sum n + floor_s + floor_t.  All values are of type seq_t. */
    return (seq_t)n + floor_s + floor_t;
}
