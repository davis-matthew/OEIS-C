/*
 * A015192  Sum of (Gaussian) q-binomial coefficients for q=-23.
 * https://oeis.org/A015192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} [n choose k]_{q=-23}
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ∑ [n choose k]_q is computed with the recurrence
 * [m choose k] = [m-1 choose k] + q^(m-k) * [m-1 choose k-1], 
 * with [n choose 0] = [n choose n] = 1. */

seq_t a(idx_t n)
{
    /* Static caches for a(n) and the two latest q-binomial rows */
    static seq_t *a_cache = NULL;
    static seq_t *prev_row = NULL;
    static seq_t *curr_row = NULL;
    static idx_t  max_cached = 0;

    /* First call – allocate minimal storage (size 1) */
    if (!a_cache) {
        a_cache   = (seq_t*)malloc(sizeof(seq_t));
        prev_row  = (seq_t*)malloc(sizeof(seq_t));
        curr_row  = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0]   = (seq_t)1;   /* a(0) = 1 */
        prev_row[0]  = (seq_t)1;   /* [0 choose 0] = 1 */
        max_cached   = 0;
    }

    /* Return cached value if already computed */
    if (n <= max_cached)
        return a_cache[n];

    /* Grow the caches to hold all needed indices up to n */
    idx_t new_size = n + 1;
    a_cache  = (seq_t*)realloc(a_cache,  new_size * sizeof(seq_t));
    prev_row = (seq_t*)realloc(prev_row, new_size * sizeof(seq_t));
    curr_row = (seq_t*)realloc(curr_row, new_size * sizeof(seq_t));

    const seq_t Q = (seq_t)-23;   /* fixed q value */

    /* Compute from max_cached+1 up to n */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        /* First and last entries of the row are 1 */
        curr_row[0] = (seq_t)1;
        curr_row[m] = (seq_t)1;

        /* Compute interior entries from k=m-1 down to 1 */
        seq_t powq = (seq_t)1;   /* will hold q^(m-k) */
        for (idx_t k = m - 1; k > 0; --k) {
            powq *= Q;                           /* q^(m-k) */
            curr_row[k] = prev_row[k] + powq * prev_row[k - 1];
        }

        /* Sum the row to obtain a(m) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k)
            sum += curr_row[k];
        a_cache[m] = sum;

        /* Prepare for the next iteration: swap rows */
        seq_t *tmp   = prev_row;
        prev_row    = curr_row;
        curr_row    = tmp;
    }

    max_cached = n;
    return a_cache[n];
}
