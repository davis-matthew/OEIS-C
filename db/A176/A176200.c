/*
 * A176200  A symmetrical triangle T(n, m) = 2*Eulerian(n+1, m) -1, read by
 *          rows.
 * https://oeis.org/A176200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*Eulerian(n+1, m) – 1, where m is the column of the
 *            triangular array read by rows with offset 0 (OEIS A176200)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 83, u64 n <= 218,
 *            u128 n <= 608, big any n
 * Validated: standard, pass, 71 terms through n = 5044 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute Eulerian number E(n, k) for n >= 1, 0 <= k < n. */
static seq_t
eulerian_n_k(idx_t n, idx_t k)
{
    if (k >= n)
        return (seq_t)0;
    if (n == 1)
        return (k == 0) ? (seq_t)1 : (seq_t)0;

    /* For n up to ~150 (needed for n <= 10000) a fixed size is safe. */
    const idx_t MAX_N = 200;
    seq_t prev[MAX_N];
    seq_t curr[MAX_N];

    /* Initialise row 1. */
    for (idx_t i = 0; i < MAX_N; ++i)
        prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;  /* E(1,0) = 1 */

    /* Build rows 2..n. */
    for (idx_t i = 2; i <= n; ++i) {
        for (idx_t j = 0; j < i; ++j)
            curr[j] = (seq_t)0;

        curr[0] = (seq_t)1;                 /* E(i,0) = 1 */
        for (idx_t j = 1; j < i - 1; ++j) {
            seq_t t1 = (seq_t)(j + 1) * prev[j];
            seq_t t2 = (seq_t)(i - j) * prev[j - 1];
            curr[j] = t1 + t2;
        }
        curr[i - 1] = (seq_t)1;             /* E(i,i-1) = 1 */

        /* Copy computed row into prev for next iteration. */
        for (idx_t j = 0; j < i; ++j)
            prev[j] = curr[j];
    }
    return prev[k];
}
/* idx_t and seq_t are provided by the build system. */

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Grow high until its offset exceeds n. */
    while ((high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search for the largest r with offset(r) <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function required by the build. */
seq_t
a(idx_t n)
{
    idx_t row     = find_row(n);
    idx_t offset  = row * (row + 1) / 2;
    idx_t col     = n - offset;          /* 0 <= col < row+1 */

    seq_t e = eulerian_n_k(row + 1, col);
    return (seq_t)2 * e - (seq_t)1;
}
