/*
 * A060538  Square array read by antidiagonals of number of ways of dividing
 *          n*k labeled items into n labeled boxes with k items in each box.
 * https://oeis.org/A060538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (i*j)!/(j!^i) where (i,j) are the indices of the *
 *            element at position n when the table T(i,j) = (i*j)!/(j!^i) is *
 *            read by antidiagonals (first the 1×1 term, then 1×2, 2×1, 1×3, *
 *            2×2, 3×1, …). The antidiagonal enumeration produces pairs *
 *            (i,j) with i+j = s, i ≥ 1, j ≥ 1. For a given ind...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Compute floor(sqrt(n)) for a 64‑bit integer n.  */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Compute T(n,k) = (n*k)!/(k!^n) via the recurrence
 * T(1,k) = 1,  T(n,k) = T(n-1,k) * C(n*k, k). */
static seq_t
T(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t t = 1; t <= n; ++t)
        res = res * binomial(t * k, k);
    return res;
}

/* Main entry for OEIS A060538. */
seq_t
a(idx_t n)
{
    /* Find minimal s with (s-1)*s/2 >= n. */
    idx_t val   = 1 + 8 * n;                  /* 1+8n fits for all intended n   */
    idx_t isqrt = floor_isqrt_u64(val);      /* floor sqrt of val               */
    idx_t s     = (isqrt + 1) / 2;            /* lower bound on the diagonal     */
    if (s * (s - 1) / 2 < n)                  /* adjust if necessary             */
        ++s;

    /* Number of terms before diagonal s. */
    idx_t before = (s - 2) * (s - 1) / 2;
    idx_t pos    = n - before;                /* 1‑based position on this diagonal */

    idx_t i = pos;          /* row index (>=1) */
    idx_t j = s - pos;      /* column index (>=1) */

    return T(i, j);
}
