/*
 * A215078  Triangle of sums of the first k n-th powers multiplied by
 *          binomial(n,k), read by rows.
 * https://oeis.org/A215078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k) * sum_{j=1..k} j^n a(n) is the triangle T
 *            read by rows (row 0 has length 1, row 1 length 2, …)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 60, u64 n <= 144, u128 n <= 367,
 *            big any n
 * Validated: standard, pass, 82 terms through n = 947 (2026-11-01)
 * Helpers:   binomial, ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
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

/* compute the row index r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
static inline idx_t row_of_idx(idx_t idx) {
    /* use the quadratic formula r = floor((sqrt(8*idx+1)-1)/2) */
    __int128 v = (__int128)8*idx + 1;
    seq_t s = floor_sqrt((seq_t)v);
    idx_t r = ((idx_t)s - 1u) / 2u;

    /* adjust for possible rounding errors */
    while (((__int128)r*(r+1)) / 2 > idx) --r;
    while (((__int128)(r+1)*(r+2)) / 2 <= idx) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Special case: first element is 0 (T(0,0)) */
    if (n == 0u) return 0;

    idx_t r    = row_of_idx(n);                    /* row index */
    idx_t idx_before = ((idx_t)r*(r+1)) / 2;        /* terms before this row */
    idx_t k    = n - idx_before;                   /* column in the row */

    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        sum += ipow((seq_t)j, (seq_t)r);
    }

    seq_t bin = binomial(r, k);
    return bin * sum;
}
