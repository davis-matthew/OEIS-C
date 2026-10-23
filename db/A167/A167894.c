/*
 * A167894  Expansion of g.f.: 1/(Sum_{k >= 0} k!*x^k).
 * https://oeis.org/A167894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167894
   Formula: a(0) = 1
            a(n) = - Σ_{i=0}^{n-1} a(i) * (n-i)! ,  n > 0   */

#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* malloc, realloc */

/* Helper to grow an array of seq_t to contain at least 'needed' elements. */
static void ensure_cache(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* External linkage required. */
seq_t a(idx_t n)
{
    /* Static caches for computed values and factorials. */
    static seq_t *a_cache  = NULL;
    static size_t a_cap    = 0;
    static size_t a_size   = 0;   /* number of elements stored in a_cache */

    static seq_t *fact_cache = NULL;
    static size_t fact_cap   = 0;
    static size_t fact_size  = 0;   /* number of factorials stored (fact_cache[0] = 0! = 1) */

    /* Ensure space for at least n+1 entries. */
    ensure_cache(&a_cache,  &a_cap,   n + 1);
    ensure_cache(&fact_cache,&fact_cap, n + 1);

    /* Initialize base case if needed. */
    if (a_size == 0) {
        a_cache[0] = 1;
        a_size = 1;
    }
    if (fact_size == 0) {
        fact_cache[0] = 1;   /* 0! = 1 */
        fact_size = 1;
    }

    /* Compute missing factorials up to n. */
    for (idx_t k = fact_size; k <= n; ++k) {
        fact_cache[k] = fact_cache[k - 1] * (seq_t)k;
    }
    fact_size = n + 1;

    /* Compute sequence values up to n. */
    for (idx_t m = a_size; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            sum += a_cache[i] * fact_cache[m - i];
        }
        a_cache[m] = -sum;
    }
    a_size = n + 1;

    return a_cache[n];
}
