/*
 * A025581  Triangle read by rows: T(n, k) = n-k, for 0 <= k <= n.
 * https://oeis.org/A025581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = r*(r+3)/2 - n, where r = floor((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 1753, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Validated: standard, pass, 118 terms through n = 4863 (2026-11-01)
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

/* Returns the n-th term of OEIS A025581 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)1 + (seq_t)8 * nn;        /* 1 + 8*n */
    seq_t s   = floor_sqrt(val);                /* floor(sqrt(1+8*n)) */
    seq_t r   = (s - (seq_t)1) / (seq_t)2;      /* row index */
    seq_t tmp = r * (r + (seq_t)3);
    tmp = tmp / (seq_t)2;                       /* r*(r+3)/2 */
    seq_t result = tmp - nn;                    /* subtract n */
    return result;
}
