/*
 * A178568  Triangle read by rows, antidiagonals of an array (row r >= 1,
 *          column n >= 1) generated from a(2n) = r*a(n), a(2n+1) = a(n) +
 *          a(n+1).
 * https://oeis.org/A178568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite lower‑triangular *
 *            matrix whose rows r (r ≥ 1) are defined by the recurrence * *
 *            a(2k) = r · a(k) * a(2k+1) = a(k) + a(k+1) (a(1)=1) * * The
 *            sequence is formed by traversing the matrix antidiagonally: *
 *            for each antidiagonal (sum of indices s = r+n ≥ 2) th...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------- integer square root (floor) of a 64‑bit number ----------- */
static unsigned long long floor_sqrt_u64(unsigned long long v)
{
    unsigned long long lo = 0, hi = 1ULL << 32;           /* 2^32 > sqrt(2^64) */
    while (lo <= hi)
    {
        unsigned long long mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == v)  return mid;
        if (sq <  v)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* ----------- compute a_r(k) = value in row r, column k (k ≥ 1) --------- */
static seq_t row_value(idx_t r, idx_t k)
{
    if (k == 1)                     /* base case */
        return (seq_t)1;

    if ((k & 1ULL) == 0)            /* even column: a(2m) = r·a(m) */
    {
        seq_t sub = row_value(r, k >> 1);
        return (seq_t)r * sub;
    }
    else                            /* odd column: a(2m+1) = a(m)+a(m+1) */
    {
        idx_t m = k >> 1;           /* (k-1)/2  */
        seq_t a1 = row_value(r, m);
        seq_t a2 = row_value(r, m + 1);
        return a1 + a2;
    }
}

/* ------------------------------ public function ----------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* invalid index, but offset starts at 1 */

    /* find antidiagonal number s such that (s-1)*s/2 ≥ n */
    idx_t twoN = n * 2ULL;
    idx_t t = floor_sqrt_u64(twoN);
    idx_t s = t + 1U;                /* initial guess */
    if ( (__int128)s * (s - 1) < (__int128)twoN )
        s++;                         /* adjust if necessary */

    /* entries preceding this antidiagonal */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* position inside the antidiagonal (0‑based) */
    idx_t k = n - prev - 1;

    /* map to concrete row/column */
    idx_t r   = s - 1 - k;   /* row number  (1 ≤ r ≤ s-1) */
    idx_t col = k + 1;       /* column number (1 ≤ col ≤ s-1) */

    return row_value(r, col);
}
