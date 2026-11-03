/*
 * A186225  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          pentagonal numbers. Complement of A186226.
 * https://oeis.org/A186225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((1 + sqrt(1 + 12*n*(n+1)))/6)
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 100, u64 n <= 100,
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
    seq_t nseq = (seq_t) n;                       /* n as seq_t */
    seq_t T = nseq * (nseq + (seq_t)1) / (seq_t)2;  /* T(n) = n*(n+1)/2 */
    seq_t D = (seq_t)12 * nseq * (nseq + (seq_t)1) + (seq_t)1;  /* D = 1 + 12*n*(n+1) */
    seq_t sqrtD = floor_sqrt(D);                  /* floor(sqrt(D)) */
    seq_t cnt   = (sqrtD + (seq_t)1) / (seq_t)6;   /* count of pentagonal <= T(n) */
    return nseq + cnt;                            /* a(n) = n + cnt */
}
