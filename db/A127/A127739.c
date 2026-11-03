/*
 * A127739  Triangle read by rows, in which row n contains the triangular
 *          number T(n) = A000217(n) repeated n times; smallest triangular
 *          number greater than or equal to n.
 * https://oeis.org/A127739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (t+1)(t+2)/2 where t = floor((-1 + sqrt(8*n-7))/2)
 * Range:     u8 n <= 8, u16 n <= 1912, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 82 terms through n = 3999 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* compute 8*n - 7 */
    seq_t v = (seq_t)n * (seq_t)8 - (seq_t)7;
    /* integer square root */
    seq_t s = floor_sqrt(v);
    /* t = (s - 1)/2 */
    seq_t t = (s - (seq_t)1) / (seq_t)2;
    /* a(n) = (t+1)(t+2)/2 */
    seq_t res = (t + (seq_t)1) * (t + (seq_t)2) / (seq_t)2;
    return res;
}
