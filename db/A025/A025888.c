/*
 * A025888  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A025888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-8) + a(n-10) - a(n-13) - a(n-15) - a(n-18) +
 *            a(n-23) a(0) = 1, a(n) = 0 for n < 0. The function memoizes
 *            previously computed terms in a static array so that multi‑step
 *            calls in any order are O(1) after the needed range has been
 *            filled. Only integer arithmetic and standard libr...
 * Range:     u8 n <= 429, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for already computed terms.  The cache is indexed by `n`. */
static seq_t *cache = NULL;      /* Pointer to the array of computed values */
static idx_t cached_n = 0;       /* Number of terms currently stored (highest index + 1) */
static size_t cache_cap = 0;     /* Allocated capacity of the cache array */

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Return cached value if already available. */
    if (n < cached_n) {
        return cache[n];
    }

    /* Ensure the cache can hold values up to index `n`. */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        /* Use a cast to avoid C++ conversion error. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the program. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to `n`. */
    for (idx_t i = cached_n; i <= n; ++i) {
        if (i == 0) {              /* Base case */
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t posit = 0;
        seq_t negat = 0;

        /* Positive contributions. */
        if (i >= 5)   posit += cache[i - 5];
        if (i >= 8)   posit += cache[i - 8];
        if (i >= 10)  posit += cache[i -10];
        if (i >= 23)  posit += cache[i -23];

        /* Negative contributions. */
        if (i >= 13)  negat += cache[i -13];
        if (i >= 15)  negat += cache[i -15];
        if (i >= 18)  negat += cache[i -18];

        /* The recurrence guarantees posit >= negat. */
        cache[i] = posit - negat;
    }

    cached_n = n + 1;          /* All indices up to `n` are now stored. */
    return cache[n];
}
