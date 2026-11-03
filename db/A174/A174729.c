/*
 * A174729  Triangle read by rows: T(n,k) = (1-3^n) * Eulerian(n + 1, k) +
 *          3^n.
 * https://oeis.org/A174729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) = T(r,k) with r = ⌊(sqrt(8n+1)-1)/2⌋, k = n - r(r+1)/2,
 *            T(r,k) = (1 - 3^r) * Eulerian(r + 1, k) + 3^r. Eulerian(n,k) is
 *            computed by the recurrence A(n,k) = (k+1)·A(n-1,k) +
 *            (n-k)·A(n-1,k-1), with A(1,0)=1
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard integer types for idx_t and seq_t (defined by build) */
#include <stdint.h>
#include <stddef.h>

/* compute the Eulerian number A(row,k) for 1 <= row <= 200, 0 <= k < row */
static seq_t eulerian_value(idx_t row, idx_t k)
{
    /* row is at most 200 for n up to 10000 (see analysis) */
    const idx_t MAX_ROW = 200;
    seq_t cur[MAX_ROW];
    seq_t nxt[MAX_ROW];
    /* initialise first row: A(1,0)=1 */
    cur[0] = (seq_t)1;
    idx_t curSize = 1;          /* current number of valid entries in cur */

    /* build rows 2 .. row */
    for (idx_t n = 2; n <= row; ++n) {
        for (idx_t j = 0; j < n; ++j) {
            seq_t term1 = 0;
            if (j < curSize)                 /* A(n-1,j) */
                term1 = (seq_t)(j + 1) * cur[j];
            seq_t term2 = 0;
            if (j > 0)                       /* A(n-1,j-1) */
                term2 = (seq_t)(n - j) * cur[j - 1];
            nxt[j] = term1 + term2;
        }
        curSize = n;
        for (idx_t j = 0; j < curSize; ++j)
            cur[j] = nxt[j];
    }
    return cur[k];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r and position k in the triangular array */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    /* number of elements before row r */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;                 /* 0 <= k <= r */

    /* compute 3^r */
    seq_t power3 = (seq_t)1;
    for (idx_t i = 0; i < r; ++i)
        power3 = power3 * (seq_t)3;

    /* Eulerian(r+1, k) */
    seq_t euler = eulerian_value(r + 1, k);

    /* (1 - 3^r) * Eulerian(r+1,k) + 3^r */
    seq_t result = ((seq_t)1 - power3) * euler + power3;
    return result;
}
