/*
 * A054077  Inverse of the permutation A054073 of N.
 * https://oeis.org/A054077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) reads the array of A054077 by antidiagonals; entry (row i,
 *            col j) is the position of i in A054073, namely T(k-1) + #{h<=k :
 *            {h*sqrt2} <= {i*sqrt2}} with k=i+j-1, T(k)=k(k+1)/2 (exact
 *            integer comparisons)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static long long A054077_isqrt(long long x)
{
    long long r = (long long)sqrtl((long double)x);
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* true iff frac(h*sqrt2) < frac(m*sqrt2) for h != m */
static int A054077_less(long long h, long long m)
{
    long long dx = h - m;
    long long dy = A054077_isqrt(2 * h * h) - A054077_isqrt(2 * m * m);
    if (dx >= 0 && dy >= 0) return 2 * dx * dx < dy * dy;
    if (dx < 0 && dy < 0) return 2 * dx * dx > dy * dy;
    if (dx >= 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    idx_t d = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    while (d * (d + 1) / 2 < n) ++d;
    while (d > 1 && (d - 1) * d / 2 >= n) --d;
    idx_t prev = (d - 1) * d / 2;
    idx_t i = n - prev;          /* row = value m */
    idx_t k = d;                 /* permutation index: m + col - 1 = d */
    long long m = (long long)i;
    idx_t rank = 0;
    for (long long h = 1; h <= (long long)k; ++h) {
        if (h == m || A054077_less(h, m)) ++rank;
    }
    return (seq_t)(prev + rank);
}
