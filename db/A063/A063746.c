/*
 * A063746  Triangle read by rows giving number of partitions of k (k=0 ..
 *          n^2) with Ferrers plot fitting in an n X n box.
 * https://oeis.org/A063746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of q^k in the Gaussian binomial coefficient
 *            [2r r]_q, where n indexes the flattened triangle read by rows.
 *            For each n the corresponding row r and offset k are found by
 *            solving ∑_{j=0}^{r-1} (j²+1) ≤ n < ∑_{j=0}^{r} (j²+1); then a(n)
 *            equals the number of partitions of k that fit...
 * Range:     u8 n <= 168, u16 n <= 717, u32 n <= 3084, u64 n <= 8840,
 *            u128 n <= 8840, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------
   Caching of pre‑computed rows.  rows[r] points to an array of
   seq_t of length r²+1 containing the coefficients of
   [2r r]_q.  row_sizes[r] holds r²+1.
   The arrays themselves are grown as needed.
   ------------------------------------------------------------ */
static seq_t **rows = NULL;          /* rows[r][k] */
static idx_t *row_sizes = NULL;     /* r²+1 for each r */
static idx_t rows_capacity = 0;     /* number of row slots allocated */

/* Ensure that rows[r] is allocated and filled.  The computation is
   performed with dynamic programming over bounded partitions.          */
static void ensure_row(idx_t r)
{
    /* Grow the outer arrays if required. */
    if (r >= rows_capacity) {
        size_t new_cap = (size_t)r + 1;
        seq_t **tmp_rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        idx_t   *tmp_sizes = (idx_t*)realloc(row_sizes, new_cap * sizeof(idx_t));
        if (!tmp_rows || !tmp_sizes) exit(EXIT_FAILURE);
        rows = tmp_rows;
        row_sizes = tmp_sizes;
        for (size_t i = rows_capacity; i < new_cap; ++i) {
            rows[i] = NULL;
            row_sizes[i] = 0;
        }
        rows_capacity = (idx_t)new_cap;
    }
    if (rows[r] != NULL) return;   /* already computed */

    /* Special case r == 0 */
    if (r == 0) {
        seq_t *coeff = (seq_t*)malloc(sizeof(seq_t));
        if (!coeff) exit(EXIT_FAILURE);
        coeff[0] = 1;
        rows[r] = coeff;
        row_sizes[r] = 1;
        return;
    }

    idx_t maxsum   = r * r;          /* maximum exponent k */
    idx_t maxparts = r;              /* maximum number of parts allowed */

    /* Allocate DP table: dp[sum * (maxparts+1) + parts] */
    size_t dp_len = (size_t)(maxsum + 1) * (size_t)(maxparts + 1);
    seq_t *dp = (seq_t*)calloc(dp_len, sizeof(seq_t));
    if (!dp) exit(EXIT_FAILURE);
    dp[0] = 1;   /* dp[0][0] = 1 */

    /* Classic partition DP with bounded part size and part count. */
    for (idx_t partsize = 1; partsize <= r; ++partsize) {
        for (idx_t parts = 1; parts <= r; ++parts) {
            for (idx_t sum = partsize; sum <= maxsum; ++sum) {
                dp[sum * (maxparts + 1) + parts] +=
                    dp[(sum - partsize) * (maxparts + 1) + (parts - 1)];
            }
        }
    }

    /* Aggregate over all allowed part counts to get the final coefficient. */
    seq_t *coeff = (seq_t*)malloc((size_t)(maxsum + 1) * sizeof(seq_t));
    if (!coeff) exit(EXIT_FAILURE);
    for (idx_t sum = 0; sum <= maxsum; ++sum) {
        seq_t val = 0;
        for (idx_t parts = 0; parts <= maxparts; ++parts) {
            val += dp[sum * (maxparts + 1) + parts];
        }
        coeff[sum] = val;
    }

    free(dp);
    rows[r] = coeff;
    row_sizes[r] = (idx_t)(maxsum + 1);
}

/* ------------------------------------------------------------
   Main sequence function.  It determines which row the
   flattened index n falls into, ensures that row is
   computed, then returns the value at the offset inside
   that row.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find row r such that cumulative length up to r-1 ≤ n < cumulative length up to r. */
    idx_t r = 0;
    idx_t cum = 0;   /* cumulative entries before row r */
    while (1) {
        idx_t rowlen = r * r + 1;   /* length of row r */
        if (cum + rowlen > n) break;
        cum += rowlen;
        ++r;
    }
    idx_t offset = n - cum;         /* position inside row r */

    ensure_row(r);
    return rows[r][offset];
}
