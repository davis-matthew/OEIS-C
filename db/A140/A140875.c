/*
 * A140875  Triangle read by rows: T(n,m) = gcd(n,m+2) - 2*gcd(n,m+1) -
 *          gcd(n,m), with diagonal and subdiagonal removed.
 * https://oeis.org/A140875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy, less
 * Formula:   a(n) is the nth element (offset 1) of the triangle read by rows:
 *            T(k+1,m)=gcd(k+1,m+2)-2*gcd(k+1,m+1)-gcd(k+1,m), where k≥1 and m
 *            ranges from 0 to k−1
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static idx_t row_for_index(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 >= n  (r>=1). */
    idx_t low = 1, high = 1;
    while ( (__int128)high * (high + 1) / 2 < n )
        high <<= 1;

    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( (__int128)mid * (mid + 1) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row (k) and column (m) for the nth element. */
    idx_t r = row_for_index(n);                 /* row number, 1‑based */
    idx_t start = ( (__int128)(r - 1) * r ) / 2; /* number of elements before this row */
    idx_t m_idx = n - start - 1;               /* 0‑based column index */

    seq_t n_val = (seq_t)(r + 1);              /* n in the definition: r+1 */
    seq_t m_val = (seq_t)m_idx;                /* m in the definition */

    seq_t g2 = gcd(n_val, m_val + 1);          /* gcd(n, m+1) */
    seq_t g1 = gcd(n_val, m_val + 2);          /* gcd(n, m+2) */
    seq_t g0 = gcd(n_val, m_val);              /* gcd(n, m)   */

    /* Compute T = gcd(n, m+2) - 2*gcd(n, m+1) - gcd(n, m). */
    return g1 - g2 - g2 - g0;
}
