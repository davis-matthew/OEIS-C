/*
 * A158156  Number of nondecreasing integer sequences of length 22 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p=1}^{22} P(n,p) * (sum_{r=1}^{22-p} P(n,r)) where
 *            P(n,k) is the number of partitions of n into exactly k positive
 *            parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 46, u64 n <= 194, u128 n <= 285,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t **dp_rows = NULL;        /* dp_rows[i][k] == P(i,k), 0 <= k <= 22 */
static idx_t   dp_rows_size = 0;      /* number of rows currently allocated  (size = dp_rows_size) */

/* Ensure that dp_rows has rows 0..n computed.  */
static void ensure_dp(idx_t n)
{
    if (n < dp_rows_size) return;          /* already computed */

    /* Allocate space for new rows. */
    size_t new_size = (size_t)n + 1;          /* rows 0 .. n inclusive */
    dp_rows = (seq_t**)realloc(dp_rows, new_size * sizeof(seq_t*));

    /* Allocate each new row and initialise to zero. */
    for (idx_t i = dp_rows_size; i <= n; ++i) {
#ifdef __cplusplus
        dp_rows[i] = new seq_t[23];
#else
        dp_rows[i] = (seq_t*)malloc(23 * sizeof(seq_t));
#endif
        /* initialise */
        for (int k = 0; k < 23; ++k) dp_rows[i][k] = 0;
    }

    /* Base row 0: P(0,0)=1, others 0. */
    if (dp_rows_size == 0) {
        dp_rows[0][0] = 1;
    }

    /* Compute rows that were just added. */
    idx_t start_row = (dp_rows_size == 0) ? 1 : dp_rows_size;
    for (idx_t row = start_row; row <= n; ++row) {
        /* dp[row][0] is already 0. */
        idx_t maxk = (row < 22) ? row : 22;
        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t val1 = dp_rows[row - 1][k - 1];
            seq_t val2 = 0;
            if (row >= k) val2 = dp_rows[row - k][k];
            dp_rows[row][k] = val1 + val2;
        }
    }

    dp_rows_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* OEIS offset starts at 1 */

    ensure_dp(n);

    /* prefix[k] = sum_{r=1}^{k} P(n,r) */
    seq_t prefix[23] = {0};
    for (int k = 1; k <= 22; ++k) {
        prefix[k] = prefix[k-1] + dp_rows[n][k];
    }

    seq_t result = 0;
    for (idx_t p = 1; p <= 22 && p <= n; ++p) {
        idx_t max_r = 22 - p;
        if (max_r > n) max_r = n;        /* for small n, the sum extends over all partitions of n */
        seq_t sum_r = prefix[max_r];
        result += dp_rows[n][p] * sum_r;
    }
    return result;
}
