/*
 * A174192  Expansion of g.f. (1-x+2*x^2)/((1-x)*(1-2*x-x^2)).
 * https://oeis.org/A174192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2, with a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
/* In C++ the new/delete operators allocate and construct objects */
static void ensure_cap(size_t req, seq_t **cache, size_t *cap) {
    if (*cache == NULL) {
        *cache = new seq_t[req];
        *cap   = req;
    } else if (req > *cap) {
        seq_t *new_cache = new seq_t[req];
        for (size_t i = 0; i < *cap; ++i) new_cache[i] = (*cache)[i];
        delete [] *cache;
        *cache = new_cache;
        *cap   = req;
    }
}
#else
/* In C, use malloc/realloc for plain data types */
static void ensure_cap(size_t req, seq_t **cache, size_t *cap) {
    if (*cache == NULL) {
        *cache = (seq_t*)malloc(req * sizeof(seq_t));
        if (*cache == NULL) exit(EXIT_FAILURE);
        *cap   = req;
    } else if (req > *cap) {
        seq_t *tmp = (seq_t*)realloc(*cache, req * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        *cache = tmp;
        *cap   = req;
    }
}
#endif

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   max_computed = 1;   /* highest index that has been computed */

    /* Initialise cache with the first two terms on first call */
    if (cache == NULL) {
        ensure_cap(2, &cache, &cap);
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        max_computed = 1;
    }

    /* Extend cache if needed */
    if (n > max_computed) {
        ensure_cap((size_t)n + 1, &cache, &cap);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)(2 * cache[i-1] + cache[i-2] + 2);
        }
        max_computed = n;
    }

    return cache[n];
}
