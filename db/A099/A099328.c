/*
 * A099328  Number of Catalan knight paths from (0,0) to (n,0) in the region
 *          between and on the lines y=0 and y=3. (See A096587 for the
 *          definition of Catalan knight.).
 * https://oeis.org/A099328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + 3*a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) for n > 7. Initial values: a(1)=1, a(2)=0, a(3)=1,
 *            a(4)=0, a(5)=2, a(6)=2, a(7)=8
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..7 */
    static const seq_t base[7] = {1,0,1,0,2,2,8};

    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* cache[1] .. cache[max] */
    static size_t cache_size = 0; /* highest index computed so far */
    static size_t cache_alloc = 0;/* allocated size (index+1) */

    /* Ensure the cache can hold up to index n */
    if ((size_t)n >= cache_alloc) {
        size_t new_cap = (size_t)n + 1; /* indices 0 .. n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* OOM */
        /* Zero any newly allocated slots (index 0 unused) */
        if (new_cap > cache_alloc)
            for (size_t i = cache_alloc; i < new_cap; ++i)
                new_cache[i] = 0;
        cache = new_cache;
        cache_alloc = new_cap;
    }

    /* Compute values up to n if needed */
    if ((size_t)n > cache_size) {
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            if (i <= 7) {
                cache[i] = base[i - 1];
            } else {
                seq_t pos = cache[i - 1] + cache[i - 2];
                pos += cache[i - 4] * ((seq_t)3);
                pos += cache[i - 5] + cache[i - 6];
                seq_t neg = cache[i - 3] + cache[i - 7];
                cache[i] = pos - neg;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}
