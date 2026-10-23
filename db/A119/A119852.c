/*
 * A119852  Number of ternary words with exactly one 012.
 * https://oeis.org/A119852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=6, a(5)=27
 *            a(n)=6*a(n-1)-9*a(n-2)-2*a(n-3)+6*a(n-4)-a(n-6) for n≥6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119852: number of ternary words with exactly one 012 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* Cached sequence values         */
static size_t cache_cap = 0;          /* Current capacity of cache      */
static size_t cache_len = 0;          /* Number of values stored        */

/* Ensure the cache can hold values up to index n (inclusive).  */
static void
ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;                 /* number of entries needed   */

    /* Allocate / grow if needed.  */
    if (need > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < need)
            new_cap <<= 1;                     /* double until sufficient   */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the program.  */
            abort();
        }
        cache = tmp;
        /* Initialize newly allocated part with zeros.   */
        for (size_t i = cache_len; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 3)
            cache[i] = 0;
        else if (i == 3)
            cache[i] = 1;
        else if (i == 4)
            cache[i] = 6;
        else if (i == 5)
            cache[i] = 27;
        else
            cache[i] =   (6 * cache[i-1])
                       - (9 * cache[i-2])
                       - (2 * cache[i-3])
                       + (6 * cache[i-4])
                       -      cache[i-6];
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
