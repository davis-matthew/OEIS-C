/*
 * A047913  Triangle of numbers a(n,k) = number of partitions of k such that k
 *          = n + n_1 + n_2 + ... + n_t where n_1 <= 2n and n_{i+1} <= 2n_i
 *          for all i.
 * https://oeis.org/A047913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n,n)=1; a(n,k)=Sum_{i=1}^{min(2n,k-n)} a(i,k-n) for k>n.
   The exported function returns a(n) where a(n) is the n-th term of the
   flattened triangle read column by column as described in the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static storage for the dynamic program */
static idx_t   current_max_k = 0;   /* maximum column that has been computed */
static seq_t * dp            = NULL;/* pointer to 2‑D table in flattened form */
static idx_t   stride        = 0;   /* width of the table (current_max_k+1) */

/* Ensure that dp contains all columns up to req_k (inclusive) */
static void ensure_max_k(idx_t req_k)
{
    if (req_k <= current_max_k) return;

    idx_t old_stride = stride;
    idx_t old_k      = current_max_k;

    /* New table dimensions */
    idx_t new_stride = req_k + 1;
    size_t new_size  = (size_t)new_stride * new_stride;
    seq_t *new_dp = (seq_t *)calloc(new_size, sizeof(seq_t));

    /* Copy existing entries */
    if (dp) {
        for (idx_t n = 1; n <= old_k; ++n) {
            for (idx_t k = 1; k <= old_k; ++k) {
                new_dp[n * new_stride + k] = dp[n * old_stride + k];
            }
        }
    }

    /* Compute new columns */
    for (idx_t cur = old_k + 1; cur <= req_k; ++cur) {
        for (idx_t n = 1; n <= cur; ++n) {
            if (n == cur) {
                new_dp[n * new_stride + cur] = 1;
            } else {
                seq_t sum = 0;
                idx_t limit = cur - n;
                idx_t max_i = 2 * n;
                if (max_i > limit) max_i = limit;
                for (idx_t i = 1; i <= max_i; ++i) {
                    sum += new_dp[i * new_stride + limit];
                }
                new_dp[n * new_stride + cur] = sum;
            }
        }
    }

    /* Replace old table */
    if (dp) free(dp);
    dp   = new_dp;
    stride = new_stride;
    current_max_k = req_k;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1; value undefined for n<1, but return 0 */

    /* Find column k such that k(k+1)/2 >= n */
    idx_t k = 0;
    while ((k * (k + 1)) / 2 < n) ++k;

    idx_t t_prev  = (k - 1) * k / 2;          /* triangular number of previous column */
    idx_t offset  = n - t_prev;               /* position inside column (1‑based) */
    idx_t row     = k - offset + 1;          /* row index (n') in triangular array */

    ensure_max_k(k);

    return dp[row * stride + k];
}
