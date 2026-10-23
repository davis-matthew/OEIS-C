/*
 * A137734  a(0)=1. a(n) = ceiling(n/b(n)), where b(n) is the largest value
 *          among (a(0),a(1),...,a(n-1)).
 * https://oeis.org/A137734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n)=ceil(n / max_{i<n} a(i)) for n>0
 * Range:     u8 n <= 433, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use a std::vector for caching */
    static std::vector<seq_t> cache;          /* holds a(0) … a(current)   */
    static seq_t max_so_far = 1;              /* maximum of the terms so far */
    static idx_t count = 0;                   /* number of initialized elements */

    if (count == 0) {                         /* initialise a(0) */
        cache.reserve(1);
        cache.push_back((seq_t)1);
        count = 1;
        max_so_far = (seq_t)1;
    }

    if (n < count)                            /* already known */
        return cache[(size_t)n];

    /* Ensure capacity */
    if (cache.size() <= (size_t)n)            /* grow to at least n+1 */
        cache.reserve((size_t)n + 1);

    /* Compute missing terms */
    for (idx_t i = count; i <= n; ++i) {
        seq_t val = ((seq_t)i + max_so_far - (seq_t)1) / max_so_far;  /* ceil(i / max_so_far) */
        cache.push_back(val);
        if (val > max_so_far) max_so_far = val;
    }
    count = n + 1;
    return cache[(size_t)n];

#else
    /* C implementation – use raw malloc/realloc cache */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;      /* allocated size */
    static idx_t count = 0;         /* number of initialized elements */
    static seq_t max_so_far = 1;    /* maximum of the terms so far */

    /* Initialise the cache for a(0) if necessary */
    if (count == 0) {
        capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        count = 1;
        max_so_far = (seq_t)1;
    }

    if (n < count)                     /* already known */
        return cache[n];

    /* Grow the buffer to at least n+1 elements */
    if (capacity <= n) {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = count; i <= n; ++i) {
        seq_t val = ((seq_t)i + max_so_far - (seq_t)1) / max_so_far; /* ceil(i / max_so_far) */
        cache[i] = val;
        if (val > max_so_far) max_so_far = val;
    }
    count = n + 1;
    return cache[n];
#endif
}
