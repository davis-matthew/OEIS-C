/*
 * A144225  Bordered Pascal's triangle in rectangular format.
 * https://oeis.org/A144225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (m,n) = (1,1); a(n) = 0 if m = 1 or n = 1 (border
 *            zeros); otherwise a(n) = binom(m+n-4, m-2); where (m,n) are the
 *            row and column indices of the anti‑diagonal enumeration of the
 *            Pascal rectangle R(i,j) = C(i+j-2, i-1)
 * Range:     u8 n <= 60, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

/* Find the minimal t such that t*(t+1)/2 >= n */
static idx_t triangular_index(idx_t n)
{
    idx_t lo = 0, hi = n; /* hi always safe because t <= n for n>=1 */
    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine (m, p) coordinates from anti‑diagonal ordering */
    idx_t t = triangular_index(n);                     /* diagonal index (starting 1) */
    idx_t prevT = (__uint128_t)(t - 1) * t / 2;         /* elements before this diagonal */
    idx_t offset = n - prevT - 1;                       /* zero‑based offset on diagonal */
    idx_t m = offset + 1;                               /* row index (≥1) */
    idx_t p = t - offset;                               /* column index (≥1) */

    /* Weight array definition */
    if (m == 1 && p == 1)
        return (seq_t)1;                     /* a(1) = 1 */
    if (m == 1 || p == 1)
        return (seq_t)0;                     /* border zeros */

    /* For interior positions: w(m,p) = C(m+p-4, m-2) */
    return binom(m + p - 4, m - 2);
}
