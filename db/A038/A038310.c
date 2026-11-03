/*
 * A038310  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*8^j.
 * https://oeis.org/A038310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) * 8^j * where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   binomial, ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    /* compute row index i */
    seq_t temp  = (seq_t)n * (seq_t)8 + (seq_t)1;          /* 8*n + 1       */
    seq_t s     = floor_sqrt(temp);                       /* floor(sqrt(8*n+1)) */
    idx_t i     = (idx_t)((s - (seq_t)1) / (seq_t)2);     /* i = (sqrt-1)/2 */

    /* column index j within the row */
    idx_t row_sum = i * (i + (idx_t)1) / (idx_t)2;        /* sum of first i entries  */
    idx_t j       = n - row_sum;                          /* j = n - i*(i+1)/2      */

    /* compute the triangle entry */
    seq_t val = binomial(i, j);                           /* binomial(i,j)          */
    val = val * pow10(i - j);                             /* multiply by 10^(i-j)    */
    val = val * ipow_u64_idx(8, j);                           /* multiply by 8^j         */

    return val;
}
