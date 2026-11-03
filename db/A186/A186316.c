/*
 * A186316  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and hexagonal
 *          numbers. Complement of A186315.
 * https://oeis.org/A186316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( n*(2n-1) ) ) + n (rank of the n-th hexagonal
 *            number in the interleaved list of squares and hexagonal numbers,
 *            with squares before hexagons when equal)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Offset starts at 1; undefined below */
    seq_t sq = (seq_t)n;
    seq_t two_n = sq * 2;
    seq_t H = sq * (two_n - 1);    /* H_n = n*(2n-1) */
    seq_t r = floor_sqrt(H);
    return r + sq;
}
