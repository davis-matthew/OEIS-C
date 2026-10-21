/*
 * A215525  a(0)=a(1)=1, a(n) = a(n-1) + a(a(n-2) mod n).
 * https://oeis.org/A215525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1, for n>=2, a(n) = a(n-1) + a(a(n-2) mod n)
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 187, u64 n <= 591,
 *            u128 n <= 2223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
#endif

seq_t a(idx_t n) {
    /* Ensure the cache can hold values up to n */
#if defined(__cplusplus)
    size_t prev_size = cache.size();
    if (prev_size <= n) {
        cache.resize(n + 1);
    }
#else
    size_t prev_size = cache_size;
    if (cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – aborting */
            abort();
        }
        cache = new_cache;
        cache_size = new_size;
    }
#endif

    /* Compute terms from prev_size to n inclusive */
    for (idx_t i = prev_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
#if defined(__cplusplus)
            cache[i] = 1;
#else
            cache[i] = 1;
#endif
        } else {
#if defined(__cplusplus)
            seq_t valPrev = cache[i - 1];
            seq_t valPrevPrev = cache[i - 2];
            idx_t key = (idx_t)(valPrevPrev % (seq_t)i);
            seq_t valKey = cache[key];
            cache[i] = valPrev + valKey;
#else
            seq_t valPrev = cache[i - 1];
            seq_t valPrevPrev = cache[i - 2];
            idx_t key = (idx_t)(valPrevPrev % (seq_t)i);
            seq_t valKey = cache[key];
            cache[i] = valPrev + valKey;
#endif
        }
    }
    /* Return the requested term */
#if defined(__cplusplus)
    return cache[n];
#else
    return cache[n];
#endif
}
