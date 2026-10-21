/*
 * A052982  Expansion of ( 1-x ) / ( 1-2*x-2*x^2+x^4 ).
 * https://oeis.org/A052982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=4, a(3)=10,
   a(n)=2*a(n-2)+2*a(n-1)-a(n-4) for n>=4. */
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Compute the A052982 sequence.  */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)4;
    }
    if (n == 3) {
        return (seq_t)10;
    }

    /* Use a cache to memoise the sequence.  */
#ifdef __cplusplus
    /* In C++ use a std::vector for trivial construction and reallocation. */
    static std::vector<seq_t> cache = { (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)10 };

    /* Grow the cache until it contains a(n). */
    while (cache.size() <= (size_t)n) {
        size_t idx = cache.size();          /* index of the new term (>=4) */
        seq_t val = 2 * cache[idx-2] + 2 * cache[idx-1] - cache[idx-4];
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    /* In C use malloc/realloc.  The cast is required for C++. */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;   /* number of values stored */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Initialise with the first four terms. */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { abort(); }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)10;
        cache_used = 4;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_used) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap = new_cap ? new_cap * 2U : 4U;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;
        cache_cap = new_cap;

        /* Compute new terms. */
        for (size_t k = cache_used; k <= (size_t)n; ++k) {
            /* a[k] = 2*a[k-2] + 2*a[k-1] - a[k-4]  (valid for k>=4) */
            cache[k] = 2 * cache[k-2] + 2 * cache[k-1] - cache[k-4];
        }
        cache_used = (size_t)n + 1;
    }

    return cache[(size_t)n];
#endif
}
