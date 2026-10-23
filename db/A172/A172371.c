/*
 * A172371  Antidiagonal triangle T(n, k) = t(k, n-k+1) where the square array
 *          t(n,k) is defined by t(n, k) = k*t(n-2, k) + t(n-3, k), t(0, k) =
 *          0, and t(1, k) = t(2, k) = 1, read by rows.
 * https://oeis.org/A172371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(k, r- k + 1) where r = floor((sqrt(8*n + 1) - 1) / 2),
 *            k = n - r*(r+1)/2. The square array t(n,k) is defined by t(0,k)
 *            = 0, t(1,k) = t(2,k) = 1, t(n,k) = k * t(n-2,k) + t(n-3,k) (n ≥
 *            3)
 * Range:     u8 n <= 86, u16 n <= 202, u32 n <= 483, u64 n <= 1208,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the value of t(row, col) where row ≥ 0, col ≥ 0. */
static inline seq_t compute_t(idx_t row, idx_t col)
{
    /* Enough space for the maximum row we ever need (≤ 142 for n ≤ 10⁴). */
    seq_t t[200];
    t[0] = (seq_t)0;
    if (row >= 1) t[1] = (seq_t)1;
    if (row >= 2) t[2] = (seq_t)1;
    for (idx_t i = 3; i <= row; ++i) {
        /* Use 128‑bit arithmetic to avoid intermediate overflow when
           seq_t is 64‑bit. The cast to seq_t truncates correctly when
           the value exceeds the capacity of the target type. */
        __int128 prod = (__int128)col * (__int128)t[i-2];
        t[i] = (seq_t)(prod + t[i-3]);
    }
    return t[row];
}

/* Find the row r and the start index of that row for a given linear
   index n (0‑based).  The loop runs ≤ 150 iterations for n ≤ 10⁴. */
static inline void row_and_offset(idx_t n, idx_t *row, idx_t *offset)
{
    idx_t r = 0;
    idx_t start = 0;
    while (start + r + 1 <= n) {
        start += r + 1;
        ++r;
    }
    *row = r;
    *offset = n - start;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_and_offset(n, &r, &k);            /* r = row, k = column in the antidiagonal */
    idx_t row_t = k;                       /* first index of t */
    idx_t col_t = r - k + 1;               /* second index of t */
    return compute_t(row_t, col_t);
}
