/*
 * A020981  Expansion of 1/((1-8*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A020981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020981: Expansion of 1/((1-8*x)*(1-11*x)*(1-12*x))
   Formula: a(n) = 31*a(n-1) - 316*a(n-2) + 1056*a(n-3)   (n > 2)
   Initial values: a(0)=1, a(1)=31, a(2)=645
   The function is memoised; it works correctly for any index n ≥ 0
   and for all supported seq_t types (small unsigned ints or
   arbitrary‑precision integer classes). */
#include <stdlib.h>
#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – vector handles construction/reallocation */
    static std::vector<seq_t> cache;
    /* initialise first three terms on first call */
    if (cache.size() == 0) {
        cache.reserve(3);
        cache.push_back((seq_t)1);   /* a(0) */
        cache.push_back((seq_t)31);  /* a(1) */
        cache.push_back((seq_t)645); /* a(2) */
    }

    /* grow the cache if necessary */
    if (n >= cache.size()) {
        cache.resize(n + 1);                /* new slots default‑constructed */
        for (idx_t i = cache.size() / 2; i <= n; ++i) { /* start from previous size */
            /* compute the new term */
            cache[i] =
                (seq_t)31 * cache[i - 1]
              - (seq_t)316 * cache[i - 2]
              + (seq_t)1056 * cache[i - 3];
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation – use malloc/realloc on a plain array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements written */
    static idx_t cache_cap  = 0;   /* current capacity */

    /* initialise first three terms on first call */
    if (cache_cap == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);    /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)645;
        cache_size = 3;
    }

    /* grow the cache if necessary */
    if (n >= cache_size) {
        /* double capacity until it covers n */
        idx_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)31;
            } else if (i == 2) {
                cache[i] = (seq_t)645;
            } else {
                cache[i] =
                    (seq_t)31 * cache[i - 1]
                  - (seq_t)316 * cache[i - 2]
                  + (seq_t)1056 * cache[i - 3];
            }
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
#endif
}
