/*
 * A062108  a(n) = floor(n^(3/4)).
 * https://oeis.org/A062108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/4))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n3   = nseq * nseq * nseq;   // n^3
    seq_t r    = floor_sqrt(n3);       // floor((n^3)^(1/2)) = floor(n^(3/2))
    seq_t res  = floor_sqrt(r);        // floor((n^(3/2))^(1/2)) = floor(n^(3/4))
    return res;
}
