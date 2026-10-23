/*
 * A168281  Triangle T(n,m) = 2*(min(n - m + 1, m))^2 read by rows.
 * https://oeis.org/A168281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2 * (min(m, r-m+1))^2 where r is the row containing the
 *            n-th flattened entry and m is the column index within that row
 *            (1‑based)
 * Range:     u8 n <= 264, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the starting linear index (1‑based) of row r: */
static inline idx_t offset_row(idx_t r)
{
    /* (r-1)*r/2 + 1  using 128‑bit arithmetic to avoid overflow */
    return (idx_t)(((unsigned __int128)(r - 1) * r) / 2) + 1;
}

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t low = 1, high = 1;
    /* Grow high until the start of that row exceeds n */
    while (offset_row(high) <= n)
        high <<= 1;
    /* Binary search for the correct row */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1; /* upper mid */
        if (offset_row(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    /* Compute column index within the row (0‑based) */
    idx_t start = offset_row(r);          /* first index of this row */
    idx_t col   = n - start;              /* 0‑based column index */
    idx_t m     = col + 1;                /* 1‑based column index */
    /* Determine s = min(m, r-m+1) */
    idx_t s1 = m;
    idx_t s2 = r - col;   /* r - (m-1) = r - col */
    idx_t s = (s1 < s2) ? s1 : s2;
    /* Compute the value: 2*s*s */
    return (seq_t)2 * (seq_t)s * (seq_t)s;
}
