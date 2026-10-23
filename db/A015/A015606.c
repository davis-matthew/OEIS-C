/*
 * A015606  a(n) = 11*a(n-1) + 10*a(n-2).
 * https://oeis.org/A015606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 10*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for cache */
    static std::vector<seq_t> cache;
    /* initialise first two terms if needed */
    if (cache.size() < 2) {
        cache.resize(2);
        cache[0] = 0;
        cache[1] = 1;
    }

    /* If the caller requests a value beyond what we have cached, grow the vector
       and compute all missing terms. */
    if (n >= cache.size()) {
        const idx_t old_size = cache.size();     /* indices that are already computed */
        cache.resize((size_t)n + 1);             /* ensure capacity up to n */
        for (idx_t i = old_size; i <= n; ++i) {  /* compute only new indices */
            cache[(size_t)i] = (seq_t)(11 * cache[(size_t)(i - 1)] +
                                      10 * cache[(size_t)(i - 2)]);
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using a malloc‑managed array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */

    /* First call: allocate space for the two base cases */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();          /* handle allocation failure */
        cache[0] = 0;
        cache[1] = 1;
    }

    /* If more entries are required, grow the array */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;                    /* indices already computed */
        idx_t new_size = n + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_ptr) abort();
        cache = new_ptr;
        cache_size = new_size;

        /* Compute all newly required terms, starting at old_size */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = (seq_t)(11 * cache[i - 1] + 10 * cache[i - 2]);
        }
    }

    return cache[n];
#endif
}
