/*
 * A060016  Triangle T(n,k) = number of partitions of n into k distinct parts,
 *          1 <= k <= n.
 * https://oeis.org/A060016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   T(n,k) = T(n-k,k) + T(n-k,k-1), T(0,0)=1, T(n,0)=0. a(n) =
 *            T(s,k) with s = smallest integer such that s(s+1)/2 ≥ n, k = n -
 *            s(s-1)/2. (Flattening row‑wise starting with row 1.)
 * Range:     u8 n <= 707, u16 n <= 4377, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   _offset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/_offset.h"

/* dynamic storage for the triangle values */
static seq_t *dp          = NULL;  /* flattened triangle */
static idx_t  dp_rows     = 0;     /* highest sum n computed (row index) */
static size_t dp_size     = 0;     /* total allocated elements */

/* allocate / extend the DP array up to the required number of rows */
static void _ensure_rows(idx_t needed_rows)
{
    if (needed_rows <= dp_rows) return;            /* already done */

    /* new storage size (include row 0 element) */
    size_t new_size = (size_t)needed_rows * (needed_rows + 1) / 2 + 1;

    seq_t *new_dp = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_dp) return;                          /* out of memory */

    /* copy old data */
    if (dp) {
        size_t old_size = (size_t)dp_rows * (dp_rows + 1) / 2 + 1;
        memcpy(new_dp, dp, old_size * sizeof(seq_t));
        free(dp);
    }

    /* initialise base T(0,0) if this is the first allocation */
    if (needed_rows >= 0)
        new_dp[0] = (seq_t)1;                     /* T(0,0) */

    /* compute triangle rows from old_rows+1 up to needed_rows */
    idx_t start_row = dp_rows + 1;                 /* first new row */
    for (idx_t n = start_row; n <= needed_rows; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t val = 0;
            idx_t nmk = n - k;                     /* n‑k */

            /* T(n-k,k) term */
            if (nmk >= k) {
                val += new_dp[_offset(nmk) + k];
            }

            /* T(n-k,k‑1) term */
            if (k == 1) {
                if (nmk == 0) val += (seq_t)1;
            } else {
                if (nmk >= k - 1) {
                    val += new_dp[_offset(nmk) + (k - 1)];
                }
            }

            new_dp[_offset(n) + k] = val;
        }
    }

    /* set new global state */
    dp      = new_dp;
    dp_rows = needed_rows;
    dp_size = new_size;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* sequence is 1‑based */

    /* determine the row s with s(s+1)/2 ≥ n */
    idx_t s = 1;
    while (((s * (s + 1)) / 2) < n) ++s;

    _ensure_rows(s);               /* make sure triangle up to row s is ready */

    idx_t k = n - _offset(s);      /* column index (1‑based) */
    return dp[_offset(s) + k];
}
