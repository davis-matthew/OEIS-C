/*
 * A181133  a(n) = n + A003056(n).
 * https://oeis.org/A181133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 1866, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t tn = (seq_t)n * 8 + 1;
    seq_t s = floor_sqrt(tn);
    seq_t diff = (s - 1) / 2;
    return (seq_t)n + diff;
}
