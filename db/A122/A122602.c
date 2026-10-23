/*
 * A122602  a(1) = 1; a(2) = 0; a(3) = 0; a(4) = 0; a(5) = 0; a(6) = 0; a(7) =
 *          0; a(8) = 0; a(9) = 0; a(10) = 0; a(n) = a(n - 1) + 9a(n - 2) -
 *          8a(n - 3) - 28a(n - 4) + 21a(n - 5) + 35a(n - 6) - 20a(n - 7) -
 *          15a(n - 8) + 5a(n - 9) + a(n - 10) for n >= 11.
 * https://oeis.org/A122602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 9a(n-2) - 8a(n-3) - 28a(n-4) + 21a(n-5) +
 *            35a(n-6) - 20a(n-7) - 15a(n-8) + 5a(n-9) + a(n-10); a(1) = 1,
 *            a(2) = … = a(10) = 0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ------------- Memoisation cache ------------- */
static seq_t *cache      = NULL;  /* 0‑unused, cache[i] holds a(i) */
static idx_t  cache_cap  = 0;     /* allocated size (index 0 .. cache_cap-1) */
static idx_t  cache_max  = 0;     /* largest n already computed */

/*  Ensure the cache can answer a(n) up to `needed`.  */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_max)
        return;                      /* already available */

    /* Expand array if necessary. */
    if (needed + 1 > cache_cap) {
        size_t new_cap = (size_t)(needed + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                 /* allocation failure */
        /* Zero‑initialise new entries. */
        for (idx_t i = cache_cap; i <= needed; ++i)
            new_cache[i] = 0;
        cache       = new_cache;
        cache_cap   = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = cache_max + 1; i <= needed; ++i) {
        if (i == 1)
            cache[i] = 1;
        else if (i <= 10)
            cache[i] = 0;
        else {
            /* Positive contributions. */
            seq_t pos = cache[i-1] + 9      * cache[i-2]
                     + 21     * cache[i-5]
                     + 35     * cache[i-6]
                     + 5      * cache[i-9]
                     +          cache[i-10];
            /* Negative contributions. */
            seq_t neg = 8      * cache[i-3]
                     + 28     * cache[i-4]
                     + 20     * cache[i-7]
                     + 15     * cache[i-8];
            cache[i] = pos - neg;
        }
    }
    cache_max = needed;
}

/*  Public entry point.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
