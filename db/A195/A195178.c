/*
 * A195178  a(n) = 4*n - floor(2*n*sqrt(2)).
 * https://oeis.org/A195178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - floor(2*n*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t m = n_seq * n_seq;   // n²
    m *= (seq_t)8;             // 8*n² = (2*n)² * 2
    seq_t s = floor_sqrt(m);   // floor( sqrt(8*n²) ) = floor(2*n*sqrt(2) )
    return (seq_t)4 * n_seq - s;
}
