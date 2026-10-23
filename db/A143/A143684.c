/*
 * A143684  a(0) = a(1) = 0; thereafter a(n) = 2*a(n-1)*a(n-2) + 1.
 * https://oeis.org/A143684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0; thereafter a(n) = 2*a(n-1)*a(n-2) + 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/*
 * Compute OEIS sequence A143684.
 * The function is thread-unsafe but satisfies the requirements:
 * - O(1) per call after initial generation,
 * - caches results for arbitrary ordering of calls,
 * - works with 8,16,32,64,128-bit unsigned types or arbitrary-precision
 *   integer types (when compiled as C++).
 */
seq_t a(idx_t n)
{
    /* Handle the two predefined base cases. */
    if (n < 2)
        return 0;

#ifdef __cplusplus
    /* C++ implementation using std::vector for caching. */
    static std::vector<seq_t> cache;
    static idx_t max_computed = 1;      /* index of the largest computed term */

    /* Ensure cache has at least the first two terms. */
    if (cache.empty())
    {
        cache.resize(2);
        cache[0] = 0;
        cache[1] = 0;
    }

    /* Extend cache if necessary. */
    if (n > max_computed)
    {
        if (cache.size() <= (size_t)n)
            cache.resize((size_t)n + 1);

        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            seq_t prod = cache[k - 1] * cache[k - 2];
            prod *= 2;
            prod += 1;
            cache[k] = prod;
        }
        max_computed = n;
    }

    return cache[(size_t)n];
#else
    /* C implementation using a simple dynamic array. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of elements currently allocated */
    static idx_t max_computed = 1; /* index of the largest computed term */

    /* Helper to allocate or reallocate the cache array. */
    void ensure_cache_size(size_t needed)
    {
        if (cache_size >= needed)
            return;
        size_t new_size = (cache_size == 0) ? 2 : cache_size;
        while (new_size < needed)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort(); /* memory allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize the first two terms if needed. */
    if (!cache)
        ensure_cache_size(2);
    if (cache_size >= 2 && max_computed < 1)
    {
        cache[0] = 0;
        cache[1] = 0;
        max_computed = 1;
    }

    /* Extend the cache up to n if required. */
    if (n > max_computed)
    {
        ensure_cache_size((size_t)n + 1);
        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            seq_t prod = cache[k - 1] * cache[k - 2];
            prod *= 2;
            prod += 1;
            cache[k] = prod;
        }
        max_computed = n;
    }

    return cache[(size_t)n];
#endif
}
