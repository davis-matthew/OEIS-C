/*
 * A083047  Square table read by antidiagonals forms a permutation of the
 *          natural numbers: T(n,0) = floor(n*x/(x-1))+1, T(n,k+1) =
 *          ceiling(x*T(n,k)), where x = (sqrt(5)+1)/2, n>=0, k>=0.
 * https://oeis.org/A083047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the square table T(n,k) by antidiagonals. T(n,0) =
 *            floor(n*x/(x-1)) + 1 where x = (sqrt(5)+1)/2. T(n,k+1) = ceil(x
 *            * T(n,k)), n>=0, k>=0. Indexing starts at n = 0
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Integer square root for 64‑bit values (used for sqrt of 8*n+1). */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = (v >> 1) + 1;
    if (v < 4) return v;             /* 0,1,2,3  -> |v| */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Ceil(phi * m) where phi = (1+sqrt(5))/2
   Implements: ceil(phi*m) = floor( (m + floor(m*sqrt(5))) / 2 ) + 1   */
static seq_t ceil_mul_phi(seq_t m)
{
    /* floor(m*sqrt(5)) = floor( sqrt(5 * m * m) )              */
    seq_t m2 = m * m;                 /* may grow beyond 128‑bit if seq_t is large enough */
    m2 *= (seq_t)5;
    seq_t msqrt5 = floor_sqrt(m2);
    seq_t num = m + msqrt5;           /* even integer or odd, integer division truncates */
    seq_t half = num >> 1;            /* floor(num/2)   */
    return half + 1;
}

/* First column of the table: T(row,0) = floor( (3*row + floor(row*sqrt(5))) / 2 ) + 1 */
static seq_t first_col(idx_t row)
{
    seq_t r = row;
    seq_t r2 = r * r;                 /* r small for the problem domain (≤ 140) */
    r2 *= (seq_t)5;                   /* 5 * row^2  */
    seq_t fsqrt5 = floor_sqrt(r2);    /* floor(row * sqrt(5)) */
    seq_t num = (seq_t)3 * r + fsqrt5;
    seq_t val = (num >> 1) + 1;       /* floor(num/2) + 1   */
    return val;
}

/* Compute T(row,col) via the recurrence using ceil_mul_phi */
static seq_t table_value(idx_t row, idx_t col)
{
    seq_t val = first_col(row);
    for (idx_t i = 0; i < col; ++i) {
        val = ceil_mul_phi(val);
    }
    return val;
}

/* The main function requested by the test harness   */
seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t m = 8 * n + 1;
    idx_t s = (isqrt_u64(m) - 1) / 2;

    idx_t start = s * (s + 1) / 2;
    idx_t t = n - start;              /* row index   */
    idx_t k = s - t;                  /* column index*/

    return table_value(t, k);
}
