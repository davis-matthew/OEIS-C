/*
 * A215696  a(n)=smallest positive k>n+2 such that k*n+1 is a square.
 * https://oeis.org/A215696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n+2 such that k*n + 1 is a perfect square
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Search k from n+3 upwards until k*n+1 is a square. */
    for (idx_t k = n + 3; ; ++k) {
        seq_t t = (seq_t)k * (seq_t)n + 1;
        seq_t s = floor_sqrt(t);
        if (s * s == t)   /* perfect square found */
            return (seq_t)k;
    }
}
