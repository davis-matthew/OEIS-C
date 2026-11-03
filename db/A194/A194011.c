/*
 * A194011  Natural interspersion of A002061; a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n)-th element of the array whose rows are
 * the natural interspersion of A002061, taken by antidiagonals.
 * The row r and column c (both 1‑based) entry is
 *
 *   T(r,c) = c^2 - c + 1
 *            + floor(r/2)
 *            + (sum of odd integers from 3 to the largest odd ≤ r)
 *            + 2(c-1) * (# of odd integers from 3 to the largest odd ≤ r)
 *
 * where the summation term and the count of odd integers are zero
 * when r < 3.  This expression follows directly from the recurrence
 * defining each row.
 */

#include <stdint.h>

/* Helper that returns the entry in row r, column c (1‑based) of the array. */
static seq_t array_row_col(idx_t r, idx_t c)
{
    /* Base value for row 1. */
    seq_t base = (seq_t)c * c - (seq_t)c + 1;

    if (r == 1) {
        return base;
    }

    /* Number of even indices in [2, r]. */
    idx_t even_cnt = r / 2;                /* r/2 integers */

    /* Number of odd indices ≥ 3 in [2, r] and the sum of those odds. */
    idx_t odd_cnt = 0;
    seq_t sum_odds = 0;

    if (r >= 3) {
        /* Largest odd ≤ r. */
        idx_t last_odd = (r & 1) ? r : r - 1;

        odd_cnt = ((last_odd - 3) >> 1) + 1;   /* number of terms 3,5,…,last_odd */

        /* Sum of an arithmetic progression: first=3, last=last_odd, terms=odd_cnt. */
        sum_odds = (seq_t)odd_cnt * ((seq_t)3 + (seq_t)last_odd) / 2;
    }

    /* Contributions from the odd indices to the shift (2(c-1) for each odd). */
    seq_t inc_shift = (seq_t)2 * (seq_t)(c - 1) * (seq_t)odd_cnt;

    /* Total increment beyond the base value. */
    seq_t inc = (seq_t)even_cnt + sum_odds + inc_shift;

    return base + inc;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++d;
        cumulative += d;          /* cumulative = d*(d+1)/2 */
    }

    /* Index within the d-th antidiagonal (1‑based). */
    idx_t prev_cumulative = cumulative - d;   /* (d-1)*d/2 */
    idx_t pos_in_diag   = n - prev_cumulative; /* 1 … d */

    /* r increases from 1 to d along the diagonal. */
    idx_t r = pos_in_diag;
    idx_t c = d + 1 - r;

    return array_row_col(r, c);
}
