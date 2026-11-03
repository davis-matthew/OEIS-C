/*
 * A094112  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] in which the length of the longest initial segment avoiding
 *          the 123-, the 132- and the 231-pattern is equal to k.
 * https://oeis.org/A094112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th element of the triangle T(n,k) read in row-major
 *            order where T(n,k) = n! / ((k-2)! * k) for 2 ≤ k ≤ n-1, T(n,n) =
 *            n, and T(n,1) = 0 for n ≥ 2. The function computes the term
 *            without precomputing the entire triangle
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 92, u64 n <= 211, u128 n <= 596,
 *            big any n
 * Validated: standard, pass, 72 terms through n = 9597 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0) return 0;                /* offset is 1, n≥1 in usual usage */

    /* Find the row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n. */
    seq_t m = (seq_t)8 * (seq_t)n + 1;
    seq_t s = floor_sqrt(m);             /* floor(sqrt(8n+1)) */
    idx_t r = (idx_t)((s - 1) / 2);

    seq_t tri = (seq_t)r * (seq_t)(r + 1) / 2;
    if (tri < (seq_t)n) {                /* adjust if not yet large enough */
        ++r;
        tri = (seq_t)r * (seq_t)(r + 1) / 2;
    }

    seq_t triPrev = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t k = (idx_t)n - (idx_t)triPrev; /* column in the row */

    /* Compute T(r,k) according to the formula. */
    if (r == 1)                       /* only element of the first row */
        return 1;

    if (k == 1)                       /* T(n,1) = 0 for n ≥ 2 */
        return 0;
    if (k == r)                       /* T(n,n) = n */
        return (seq_t)r;

    /* For 2 ≤ k ≤ r-1: T(r,k) = r! / ((k-2)! * k) = (product_{i=k-1}^r i) / k. */
    seq_t res = 1;
    for (idx_t i = k - 1; i <= r; ++i)
        res *= (seq_t)i;
    res /= (seq_t)k;

    return res;
}
