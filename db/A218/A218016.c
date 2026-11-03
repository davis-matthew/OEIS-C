/*
 * A218016  Triangle, read by rows, where T(n,k) = k!*C(n, k)*5^(n-k) for
 *          n>=0, k=0..n.
 * https://oeis.org/A218016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k!*C(r,k)*5^(r-k) * where r = floor((sqrt(8*n+1)-1)/2) *
 *            and k = n - r*(r+1)/2 (0 ≤ k ≤ r)
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 81, u64 n <= 221, u128 n <= 617,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 5113 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1. Find row r and column k from index n */
    seq_t tmp = (seq_t)8 * (seq_t)n + 1;        /* 8*n + 1 */
    seq_t s   = floor_sqrt(tmp);               /* floor(sqrt(8*n+1)) */
    idx_t r   = ((idx_t)s - 1) / 2;             /* r = floor((sqrt(8*n+1)-1)/2) */
    idx_t start = r * (r + 1) / 2;              /* first index of row r */
    idx_t k   = n - start;                      /* column index */

    /* 2. Compute the triangle entry T(r,k) */
    seq_t res = binomial(r, k);                 /* C(r,k) */
    res *= factorial(k);                       /* * k!   */
    res *= power5(r - k);                       /* * 5^(r-k) */

    return res;
}
