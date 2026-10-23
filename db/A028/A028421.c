/*
 * A028421  Triangle read by rows: T(n, k) = (k+1)*A132393(n+1, k+1), for 0 <=
 *          k <= n.
 * https://oeis.org/A028421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 79, u64 n <= 211,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k):
   T(0,0) = 1
   T(n,0) = n * T(n-1,0)                     (n ≥ 1)
   T(n,k) = n * T(n-1,k) + ((k+1)/k) * T(n-1,k-1)   (1 ≤ k ≤ n)
   Each a(n) is the flattened triangle read by rows:
   a(n) = T(r,k) where  n = r*(r+1)/2 + k   and 0 ≤ k ≤ r
   The function below computes a(n) directly from this definition. */

#include <stdlib.h>
#include <math.h>

/* Computes the n-th term of A028421. */
seq_t a(idx_t n)
{
    /* Find row r and column k such that n = r*(r+1)/2 + k. */
    /* Start with an initial guess using double sqrt. */
    double sqrt_val = sqrt((double)n * 8.0 + 1.0);
    size_t r = (size_t)((sqrt_val - 1.0) / 2.0);

    /* Adjust r to satisfy r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    size_t k = (size_t)n - r * (r + 1) / 2;

    /* If k > r (can happen for n out of range) return 0. */
    if (k > r) return (seq_t)0;

    /* Special case: first column is factorial. */
    if (k == 0) {
        seq_t fact = 1;
        for (size_t i = 2; i <= r; ++i)
            fact = fact * (seq_t)i;
        return fact;
    }

    /* If r == 0 we already handled k==0; otherwise r >= 1. */
    size_t maxlen = r + 1;
    seq_t *prev = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(maxlen * sizeof(seq_t));
    if (!prev || !curr) {
        /* In case of allocation failure return 0. */
        free(prev); free(curr);
        return (seq_t)0;
    }

    prev[0] = 1;                          /* T(0,0) = 1 */

    for (size_t nrow = 1; nrow <= r; ++nrow) {
        curr[0] = (seq_t)nrow * prev[0];   /* T(n,0) = n*T(n-1,0) */

        for (size_t col = 1; col <= nrow; ++col) {
            seq_t term1 = (col <= nrow - 1) ? (seq_t)nrow * prev[col] : 0;
            seq_t term2 = (seq_t)(col + 1) * prev[col - 1] / (seq_t)col;
            curr[col] = term1 + term2;     /* T(n, k) recurrence */
        }

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
