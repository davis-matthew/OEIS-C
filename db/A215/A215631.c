/*
 * A215631  Triangle read by rows: T(n,k) = n^2 + n*k + k^2, 1 <= k <= n.
 * https://oeis.org/A215631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth term of the triangle T(r,k) = r^2 + r*k + k^2, 1
 *            ≤ k ≤ r, where r is the smallest integer with r(r+1)/2 ≥ n and k
 *            = n − r(r−1)/2
 * Range:     u8 n <= 53, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r for which r(r+1)/2 ≥ n. */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;

    /* Grow hi until the triangular number exceeds or equals n. */
    while ((seq_t)hi * (hi + 1) / 2 < n)
    {
        if (hi > (idx_t)(-1) / 2)
        {
            hi = (idx_t)(-1);
            break;
        }
        hi <<= 1;
    }

    /* Binary search between lo and hi. */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t s = (seq_t)mid * (mid + 1) / 2;

        if (s >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Evaluate the triangle entry T(r,k). */
static inline seq_t triangle_value(idx_t r, idx_t k)
{
    seq_t rr = r;
    seq_t kk = k;
    return rr * rr + rr * kk + kk * kk;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; undefined for n==0 */

    idx_t r = find_row(n);
    idx_t prev = (idx_t)(((seq_t)r * (r - 1)) / 2);
    idx_t k = n - prev;           /* 1 ≤ k ≤ r */

    return triangle_value(r, k);
}
