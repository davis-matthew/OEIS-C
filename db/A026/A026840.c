/*
 * A026840  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into distinct parts <= k, for k=1,2,...n.
 * https://oeis.org/A026840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where the triangular array T is read by rows,
 *            T(n,k) = number of partitions of n into distinct parts ≤ k
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- implementation ----------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* Cached values of the sequence (1‑based indexing) */
static seq_t *seq      = NULL;          /* array of computed a(i)  */
static size_t seq_len  = 0;             /* number of values stored  */
static size_t seq_cap  = 0;             /* capacity of seq array    */
static idx_t row_done  = 0;             /* number of rows already computed */

/* Temporary DP array for the current row */
static seq_t *dp       = NULL;
static idx_t dp_cap    = 0;             /* capacity of dp array */

/* Append a new value to seq, resizing if necessary */
static void append_seq(seq_t value)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? (seq_cap << 1) : 64;
        seq_t *newptr = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!newptr) abort();      /* out of memory fatal */
        seq = newptr;
        seq_cap = newcap;
    }
    seq[seq_len++] = value;
}

/* Ensure dp has enough space for n+1 elements */
static void ensure_dp_cap(idx_t n)
{
    if (dp_cap < (idx_t)(n + 1)) {
        seq_t *newptr = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!newptr) abort();
        dp = newptr;
        dp_cap = n + 1;
    }
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    /* Compute rows until we have at least n values */
    while (seq_len < (size_t)n) {
        idx_t r = ++row_done;                 /* next row number */

        /* Prepare DP array for this row */
        ensure_dp_cap(r);
        dp[0] = 1;
        for (idx_t i = 1; i <= r; ++i) dp[i] = 0;

        /* Build partitions for this row:
           After processing each k, dp[r] holds T(r,k). */
        for (idx_t k = 1; k <= r; ++k) {
            for (idx_t s = r; ; --s) {
                dp[s] += dp[s - k];
                if (s == k) break;
            }
            append_seq(dp[r]);                 /* store T(r,k) */
        }
    }
    return seq[n - 1];
}
