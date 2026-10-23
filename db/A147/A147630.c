/*
 * A147630  a(n) = Product_{k = 1..n-1} (9*k - 3).
 * https://oeis.org/A147630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n-1} (9*k - 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 1) */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *a_cache = NULL;   /* array of precomputed terms, 1‑based */
static idx_t  a_cache_last = 0; /* largest n that has been computed */

/* Ensure that a_cache contains values up to index n. */
static void
ensure_a_cache(idx_t n)
{
    if (n <= a_cache_last)
        return;                       /* already computed */

    /* Resize cache if necessary */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (a_cache == NULL)
            abort();                 /* allocation failure */
        /* initialize a(1) */
        a_cache[1] = (seq_t)1;
        a_cache_last = 1;
    } else if (a_cache_last < n) {
        seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        a_cache = tmp;
    }

    /* Compute missing values */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        /* factor = 9*(i-1) - 3 */
        seq_t factor = (seq_t)(9 * (i - 1) - 3);
        a_cache[i] = a_cache[i - 1] * factor;
    }
    a_cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* out of bounds, return 0 */

    ensure_a_cache(n);
    return a_cache[n];
}
