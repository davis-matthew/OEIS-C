/*
 * A140060  Array of quotients.
 * https://oeis.org/A140060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A140060: the concatenation of rows of quotients Q(n,k).
   For a given index n (starting at 1), the function searches for the
   unique row r such that the cumulative length of all previous rows
   is < n ≤ cumulative length up to row r, then returns
   Q(r, n - cumulative_{r-1}), where
        Q(r,1) = r
        Q(r,k) = k * floor(Q(r,k-1)/k)   (for k ≥ 2).
   The implementation is O(n) for each call and works for any
   non‑negative index, within the time bounds for n up to about 10 000. */
#include <stdbool.h>

static idx_t row_length(idx_t n)
{
    seq_t q = (seq_t)n;
    idx_t len = 1;          /* Q(n,1) is always non‑zero */
    for (idx_t k = 2; ; ++k) {
        if (q / k == 0) break;
        q = ((seq_t)k) * (q / k);
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* n is 1‑based global index in the flattened array */
    idx_t row = 1;
    idx_t cum = 0;          /* total number of terms processed so far */

    for (;; ++row) {
        idx_t len = row_length(row);
        if (cum + len >= n) {
            /* The desired element is in this row. */
            idx_t k = n - cum;   /* column (1‑based) within the row */
            seq_t res = (seq_t)row;
            for (idx_t i = 2; i <= k; ++i)
                res = ((seq_t)i) * (res / i);
            return res;
        }
        cum += len;
    }
}
