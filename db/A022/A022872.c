/*
 * A022872  a(n) = [ a(n-1)/a(1) ] + [ a(n-3)/a(3) ] + [ a(n-5)/a(5) ] + ...,
 *          for n >= 3.
 * https://oeis.org/A022872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=2, a(3)=1, a(n)=Σ_{odd k≤n-1} a(n-k)/a(k) for n≥4
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition with memoization for arbitrary order of calls. */

#ifdef __cplusplus
    #include <vector>
    #include <cstddef>
#else
    #include <stdlib.h>
    #include <stddef.h>
#endif

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly. */
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)2;
    if (n == 3u) return (seq_t)1;

    /* Static cache to store previously computed values. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static idx_t computed_up_to = 0;
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t computed_up_to = 0;
#endif

    /* Initialize the cache with the first three terms. */
    if (computed_up_to < 3) {
#ifdef __cplusplus
        cache.resize(3);
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)1;   /* a(3) */
#else
        if (cache_cap < 3) {
            size_t new_cap = 3;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) { /* allocation failed – abort silently */
                return (seq_t)0;
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)1;   /* a(3) */
        cache_size = 3;
#endif
        computed_up_to = 3;
    }

    /* Extend the cache up to the requested index n. */
    while (computed_up_to < n) {
        idx_t i = computed_up_to + 1;   /* next index to compute (1‑based) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i - 1; k += 2) {
            idx_t idx_term = i - k;           /* term a(i‑k) */
#ifdef __cplusplus
            seq_t part = cache[idx_term - 1] / cache[k - 1];
#else
            seq_t part = cache[idx_term - 1] / cache[k - 1];
#endif
            sum += part;
        }

#ifdef __cplusplus
        cache.push_back(sum);
#else
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) { /* allocation failed – abort silently */
                return (seq_t)0;
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = sum;
#endif
        computed_up_to = i;
    }

    /* Return the cached value. */
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
