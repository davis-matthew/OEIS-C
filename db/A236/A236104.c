/*
 * A236104  Triangle read by rows: T(n,k), n >= 1, k >= 1, in which column k
 *          lists k copies of the positive squares in nondecreasing order, and
 *          the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A236104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) is the element in the triangle where column k contains k
 *            copies of each positive square, with the first element of column
 *            k appearing at row k(k+1)/2. Rows are read from left to right,
 *            and rows are concatenated to form the sequence. For a given
 *            index n: 1. Find the row r such that Σ_{i<r} L(...
 * Range:     u8 n <= 45, u16 n <= 3608, u32 n <= 9794, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t row_length(idx_t r)
{
    /* Return the largest k such that k(k+1)/2 ≤ r.  Since r ≤ ~500 for n ≤ 10000,
       a simple linear search is fast and avoids any sqrt. */
    idx_t k = 1;
    while ((k + 1) * (k + 2) / 2 <= r)
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    /* Find the row that contains position n. */
    idx_t r = 1;                 /* current row index   */
    idx_t cum = 0;               /* cumulative terms so far */

    idx_t L;                     /* length of the current row */

    while (1) {
        L = row_length(r);
        cum += L;
        if (cum >= n)
            break;
        ++r;
    }

    /* The term lies in row r, column k. */
    idx_t k = n - (cum - L);     /* 1 ≤ k ≤ L   */
    /* Position of the first element of column k. */
    idx_t t_k = k * (k + 1) / 2; /* k(k+1)/2  (fits in 64‑bit for the expected ranges) */
    /* Offset within column k. */
    idx_t offset = r - t_k;      /* ≥ 0      */
    /* Group number (1-based) whose square appears here. */
    idx_t m = offset / k + 1;

    /* Return the square of m. */
    return (seq_t)m * (seq_t)m;
}
