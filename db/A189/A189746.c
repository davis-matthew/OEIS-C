/*
 * A189746  a(1)=5, a(2)=2, a(n) = 5*a(n-1) + 2*a(n-2).
 * https://oeis.org/A189746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=5, a(2)=2, a(n) = 5*a(n-1) + 2*a(n-2) for n>=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

/* Return the n-th term of OEIS A189746.  The result is exact within
   the limits of the seq_t type.  Cached values are reused across
   calls, allowing arbitrary order of requests. */
seq_t a(idx_t n)
{
    /* n is indexed from 1.  For n<1 the behaviour is irrelevant. */
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;      /* 0‑based array of terms */
    static size_t cache_cap = 0;     /* allocated array capacity */
    static idx_t cache_size = 0;     /* number of terms stored */

    /* Lazy initialisation: ensure we can store at least the first two terms. */
    if (cache == NULL) {
        cache_cap = (size_t)(n > 2 ? n : 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* unrecoverable, sequence cannot be computed */
        cache[0] = (seq_t)5;  /* a(1) */
        cache[1] = (seq_t)2;  /* a(2) */
        cache_size = 2;
    }

    /* If the requested index lies within the cached range, return it. */
    if (n <= cache_size) return cache[n - 1];

    /* Expand the cache if necessary.  Grow exponentially to keep O(n). */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort(); /* unrecoverable */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev1 = cache[cache_size - 1];     /* a(i-1) */
        seq_t prev2 = cache[cache_size - 2];     /* a(i-2) */
        cache[cache_size] = (seq_t)((seq_t)5 * prev1 + (seq_t)2 * prev2);
        ++cache_size;
    }

    return cache[n - 1];
}
