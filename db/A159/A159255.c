/*
 * A159255  Irregular triangle read by rows: row n gives expansion of
 *          (1-x+x^2)*(1+x)^n.
 * https://oeis.org/A159255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = binom(k,j) - binom(k,j-1) + binom(k,j-2) where k is the
 *            row index with cumulative(k-1) ≤ n < cumulative(k) and j = n -
 *            cumulative(k-1). The row entries give the coefficients of (1 - x
 *            + x²)(1 + x)^k
 * Range:     i8 n <= 55, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* number of terms up to and including row k */
static inline idx_t cumulative(idx_t k)
{
    /* (k+1)(k+6)/2  – use 128‑bit arithmetic to avoid overflow */
    return (idx_t)(((unsigned __int128)(k + 1) * (k + 6)) / 2);
}

/* binomial coefficient C(n,k) computed with safe multiplication */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) safely */
        res = mul_small(res, (unsigned int)(n - k + i));
        /* exact division by i */
        res /= i;
    }
    return res;
}

/* find the row k such that cumulative(k-1) ≤ n < cumulative(k) */
static idx_t find_row(idx_t n)
{
    /* solve (k+1)(k+6)/2 > n.  Initial estimate via quadratic formula. */
    idx_t k = (idx_t)((sqrt((long double)(8 * (long double)n + 49)) - 7) / 2);

    /* adjust up if needed */
    while (cumulative(k) <= n) ++k;
    /* adjust down if overshoot */
    while (k > 0 && cumulative(k - 1) > n) --k;

    return k;
}

seq_t a(idx_t n)
{
    idx_t k   = find_row(n);
    idx_t start = (k == 0) ? 0 : cumulative(k - 1);
    idx_t j   = n - start;            /* position within the row   */

    /* coefficients: binom(k,j) - binom(k,j-1) + binom(k,j-2) */
    seq_t t1 = binom(k, j);
    seq_t t2 = (j > 0)   ? binom(k, j - 1) : 0;
    seq_t t3 = (j > 1)   ? binom(k, j - 2) : 0;

    return t1 - t2 + t3;
}
