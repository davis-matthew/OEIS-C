/*
 * A005657  a(n) = floor( tan(n)^2 ).
 * https://oeis.org/A005657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(tan(n)^2), computed in extended precision on the
 *            index; only the resulting native integer is converted to seq_t
 * Range:     u8 n <= 10, u16 n <= 6378, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    long double t = tanl((long double)n);
    long double sq = t * t;
    uint64_t m = (uint64_t)floorl(sq);
    return (seq_t)m;
}
