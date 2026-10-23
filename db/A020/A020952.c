/*
 * A020952  a(2n+1)=a(n), a(2n)=a(n)+a(n-1).
 * https://oeis.org/A020952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A020952:
   a(0) = 2, a(1) = 1
   a(2n)   = a(n) + a(n-1)
   a(2n+1) = a(n)  (for n ≥ 0)
   Offset: 0 (a(0) is the first term) */

#include <stdlib.h>   /* for malloc, realloc, free */

/* Static cache for memoisation */
static seq_t *cache       = NULL;   /* array of computed a(i) */
static size_t cache_cap   = 0;      /* current allocated capacity (in elements) */
static idx_t  computed    = 0;      /* first index that has not yet been computed */

/* Ensure that 'cache' can hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    /* If already large enough, nothing to do */
    if (cache_cap >= (size_t)(n + 1))
        return;

    /* New capacity must be at least n+1, but also at least 2 for the base cases */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 2)
        new_cap = 2;

    /* Allocate (or reallocate) the cache */
    seq_t *tmp = (cache == NULL) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                 : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Out of memory – terminate the program safely */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the nth term of the sequence, with memoisation */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold the requested index */
    ensure_cache_size(n);

    /* Initialise the base values on first use */
    if (computed == 0) {
        cache[0] = 2;   /* a(0) */
        cache[1] = 1;   /* a(1) */
        computed = 2;   /* indices 0 and 1 are now computed */
    }

    /* If the value is already known, return it immediately */
    if (n < computed)
        return cache[n];

    /* Compute all values up to and including n in ascending order */
    for (idx_t i = computed; i <= n; ++i) {
        if ((i & 1) == 0) {                /* i is even: a(2k) = a(k) + a(k-1) */
            idx_t k = i >> 1;              /* i / 2 */
            cache[i] = cache[k] + cache[k - 1];
        } else {                           /* i is odd: a(2k+1) = a(k) */
            idx_t k = i >> 1;              /* (i-1) / 2   */
            cache[i] = cache[k];
        }
    }

    /* All indices < = n are now computed */
    computed = n + 1;
    return cache[n];
}
