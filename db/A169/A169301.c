/*
 * A169301  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n > 0 a(n) = RHS(n) + 46*∑_{i=1}^{min(28,n)} a(n-i)
 *            - 1081*a(n-29), where RHS(0)=1, RHS(1…28)=2, RHS(29)=1, RHS(n)=0
 *            for n>29
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows from the rational generating function
   N(t)/D(t) given in the OEIS entry.  The function memoises
   previous values so that arbitrary calls to a(n) are O(1) after
   the first evaluation in a range. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_allocated = 0; /* allocated slots */
    static idx_t cache_max = 0;        /* highest index stored */

    /* Allocate initial cache if needed. */
    if (cache == NULL) {
        cache_allocated = 1;
        cache = (seq_t *)malloc(cache_allocated * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback if malloc fails */
        cache[0] = (seq_t)1;
        cache_max = 0;
    }

    /* Grow cache if required. Double size or just enough. */
    if ((size_t)n >= cache_allocated) {
        size_t new_alloc = cache_allocated * 2;
        if (new_alloc <= (size_t)n + 1) {
            new_alloc = (size_t)n + 1;
        }
        seq_t *new_cache = (seq_t *)malloc(new_alloc * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* fallback */
        memcpy(new_cache, cache, cache_allocated * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_allocated = new_alloc;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t rhs;
        if (i == 0) {
            rhs = (seq_t)1;
        } else if (i <= 28) {
            rhs = (seq_t)2;
        } else if (i == 29) {
            rhs = (seq_t)1;
        } else {
            rhs = 0;
        }

        seq_t sum = rhs;

        /* Add 46 * a(i-j) for j = 1 .. min(28, i). */
        idx_t limit = (i < 28 ? i : 28);
        for (idx_t j = 1; j <= limit; ++j) {
            sum += cache[i - j] * (seq_t)46;
        }

        /* Subtract 1081 * a(i-29) if applicable. */
        if (i >= 29) {
            sum -= cache[i - 29] * (seq_t)1081;
        }

        cache[i] = sum;
    }

    cache_max = n;
    return cache[n];
}
