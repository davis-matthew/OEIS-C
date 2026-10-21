/*
 * A214831  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 9.
 * https://oeis.org/A214831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 1, a(1) = a(2) = 9
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#else
/* C implementation of a dynamic array of seq_t */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure cache has room for at least `needed` elements. */
static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap < (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}
#endif

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Handle the initial three values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)9;

#ifdef __cplusplus
    /* Use a std::vector to hold the terms; it automatically constructs
       objects and manages memory. */
    static std::vector<seq_t> cache_vector;
    if (cache_vector.empty()) {
        cache_vector.reserve(3);
        cache_vector.push_back((seq_t)1);
        cache_vector.push_back((seq_t)9);
        cache_vector.push_back((seq_t)9);
    }
    if (cache_vector.size() <= (size_t)n) {
        size_t old = cache_vector.size();
        cache_vector.resize((size_t)n + 1);
        for (size_t i = old; i <= (size_t)n; ++i) {
            cache_vector[i] = cache_vector[i - 1] + cache_vector[i - 2]
                            + cache_vector[i - 3];
        }
    }
    return cache_vector[(size_t)n];
#else
    /* C implementation: use the manually managed array. */
    if (cache == NULL) {
        ensure_cache_cap(3);
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = (seq_t)9;
        cache_size = 3;
    }
    if (cache_size <= n) {
        ensure_cache_cap(n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        cache_size = n + 1;
    }
    return cache[n];
#endif
}
