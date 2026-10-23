/*
 * A132884  Triangle read by rows: T(n,k) is the number of paths in the right
 *          half-plane from (0,0) to (n,0), consisting of steps U=(1,1),
 *          D=(1,-1), h=(1,0) and H=(2,0), having k h=(1,0) steps (0<=k<=n).
 * https://oeis.org/A132884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where r = floor((sqrt(8*n+1)-1)/2), * k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 28, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

/* Compute T(n,k) – number of paths from (0,0) to (n,0) with exactly k h-steps. */
static seq_t triangle_T(idx_t n, idx_t k)
{
    /* bounds */
    if (k > n)                         return (seq_t)0;
    if (((n - k) & 1ULL) != 0)         return (seq_t)0;   /* n and k must have same parity */

    /* Even for small values handle directly */
    if (n == 0 && k == 0)              return (seq_t)1;

    /* s = (n-k)/2 is the number of (U+H) pairs needed */
    idx_t s = (n - k) >> 1;
    /* base = (n+k)/2 is the total number of steps when m=0 */
    idx_t base = (n + k) >> 1;

    seq_t sum = (seq_t)0;
    for (idx_t m = 0; m <= s; ++m)
    {
        idx_t total = base + m;            /* total steps when the number of U steps = m   */
        /* multinomial coefficient: total!/(m! m! k! (s-m)!) */
        seq_t term = binom(total, m);                      /* choose positions for U */
        term = term * binom(total - m, m);                /* choose positions for D */
        term = term * binom(total - 2 * m, k);             /* choose positions for h steps */
        /* H steps fill the remaining positions automatically */
        sum += term;
    }
    return sum;
}

/* Map linear index n to the triangle entry T(r,k). */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    double d = sqrt(1.0 + 8.0 * (double)n);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors. */
    while (r * (r + 1ULL) / 2ULL > n) --r;
    while ((r + 1ULL) * (r + 2ULL) / 2ULL <= n) ++r;

    idx_t start = r * (r + 1ULL) / 2ULL;
    idx_t k = n - start;   /* column index within the row */

    return triangle_T(r, k);
}
