/*
 * A118933  Triangle, read by rows, where T(n,k) = n!/(k!*(n-4*k)!*4^k) for
 *          n>=4*k>=0.
 * https://oeis.org/A118933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the triangle read by rows where
 *            T(r,k) = r!/(k! (r-4k)! 4^k). The row r is the smallest such
 *            that cumulative row lengths exceed n, and k = n - cumulative row
 *            lengths of rows < r
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 47, u64 n <= 103,
 *            u128 n <= 262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Euclidean algorithm for idx_t; works in C11 and C++17 */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a   = b;
        b   = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Find row r such that S(r) > n, where S(r) = (r+1) + t*(r-2t-1)
       and t = floor(r/4). */
    idx_t r = 0;
    while (1) {
        idx_t t = r / 4;
        /* Use 128‑bit arithmetic to avoid overflow */
        __int128 S = (__int128)(r + 1) + (__int128)t * (r - 2 * t - 1);
        if (S > (__int128)n) break;
        ++r;
    }

    /* Compute S(r-1) to determine k */
    idx_t S_prev;
    if (r == 0) {
        S_prev = 0;
    } else {
        idx_t rp = r - 1;
        idx_t t = rp / 4;
        S_prev = (rp + 1) + t * (rp - 2 * t - 1);
    }
    idx_t k = n - S_prev;
    if (k == 0) return (seq_t)1;          /* first element of every row */

    /* Prepare denominator factors: 4 repeated k times, then 1..k */
    size_t denom_cnt = 2 * (size_t)k;
    idx_t *denom = (idx_t *)malloc(denom_cnt * sizeof(idx_t));
    if (!denom) return (seq_t)0;           /* allocation failure fallback */
    for (size_t i = 0; i < (size_t)k; ++i) denom[i] = 4;
    for (size_t i = 0; i < (size_t)k; ++i) denom[i + (size_t)k] = (idx_t)(i + 1);

    seq_t res = (seq_t)1;
    for (idx_t num = r; num >= r - 4 * k + 1; --num) {
        idx_t temp = num;
        for (size_t i = 0; i < denom_cnt; ++i) {
            idx_t d = denom[i];
            if (d == 1) continue;
            idx_t g = gcd_idx(temp, d);
            if (g > 1) {
                temp   = temp / g;
                denom[i] = d / g;
            }
        }
        if (temp > 1) res *= (seq_t)temp;
    }

    /* Any remaining denominators should be 1; divide just in case */
    for (size_t i = 0; i < denom_cnt; ++i)
        if (denom[i] != 1) res /= (seq_t)denom[i];

    free(denom);
    return res;
}
