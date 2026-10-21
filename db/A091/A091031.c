/*
 * A091031  Third to last entries in rows of array A090452 (scaled
 *          (3,2)-Stirling2).
 * https://oeis.org/A091031
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9*n^2-19*n+8)*(n-1)*C(n))/(4*(2*n-1)) (n>=2) where C(n)
 *            is the nth Catalan number
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached Catalan numbers */
static seq_t *catalan_cache = NULL;
static size_t catalan_cap   = 0;   /* capacity of the cache array     */
static idx_t  catalan_size  = 0;   /* number of values stored          */

/* Ensure the cache holds at least (n+1) entries and compute missing values */
static void ensure_catalan(idx_t n)
{
    if (n < catalan_size) return;

    /* Grow the cache to accommodate n */
    size_t needed = (size_t)n + 1;
    if (needed > catalan_cap) {
        size_t new_cap = catalan_cap ? catalan_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        catalan_cache = (seq_t*)realloc(catalan_cache,
                                       new_cap * sizeof(seq_t));
        catalan_cap = new_cap;
    }

    /* Initialize from first missing index */
    for (idx_t i = catalan_size; i <= n; ++i) {
        if (i == 0) {
            catalan_cache[0] = (seq_t)1;
        } else {
            /* Catalan recurrence:
               C_i = C_{i-1} * (4*i-2) / (i+1)   (for i>=1) */
            seq_t prev = catalan_cache[i-1];
            seq_t num  = prev * (seq_t)(4*i - 2);
            seq_t den  = (seq_t)(i + 1);
            catalan_cache[i] = num / den;
        }
    }
    catalan_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* value below offset is irrelevant */

    /* Make sure we have C(n) ready */
    ensure_catalan(n);

    /* Compute numerator: (9*n^2 - 19*n + 8) * (n-1) * C(n) */
    seq_t term1 = (seq_t)(9*n*n - 19*n + 8);
    term1 = term1 * (seq_t)(n - 1);
    term1 = term1 * catalan_cache[n];

    /* Compute denominator: 4 * (2*n - 1) */
    seq_t denom = (seq_t)4 * (seq_t)(2*n - 1);

    /* Final result */
    return term1 / denom;
}
