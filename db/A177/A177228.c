/*
 * A177228  Triangle read by rows: T(n, k) = -binomial(n,k) for 1 <= k <= n-1,
 *          otherwise T(n, k) = 3.
 * https://oeis.org/A177228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less, easy
 * Formula:   a(n) = T(r,k) * where r is the row with offset r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, * k = n - r*(r+1)/2, * and T(r,0) = T(r,r) = 3,
 *            otherwise T(r,k) = -C(r,k)
 * Range:     i8 n <= 38, i16 n <= 177, i32 n <= 572, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binom_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_row.h"

/* --- helper: integer square root of a 128‑bit number ---------------- */
static uint64_t isqrt128(__int128 x)
{
    uint64_t low = 0, high = (uint64_t)1 << 32; /* sqrt of max 2^68 < 2^34 < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= x) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* --- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    /* find row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    __int128 t = (__int128)8 * n + 1;
    uint64_t sqrt_t = isqrt128(t);
    /* r = floor( (sqrt(8n+1)-1)/2 ) */
    idx_t r = (idx_t)((sqrt_t - 1) >> 1);

    idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    if (k == 0 || k == r) {
        return (seq_t)3;
    } else {
        seq_t val = binom_row(r, k);
        return -val;
    }
}
