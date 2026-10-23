/*
 * A025942  Expansion of 1/((1-2*x)*(1-3*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A025942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 168*a(n-2) + 468*a(n-3) - 432*a(n-4) a(0) =
 *            1, a(1) = 23, a(2) = 361, a(3) = 4907
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of elements that can be stored */
static idx_t cache_len = 0;   /* highest index for which cache[i] is valid */

/* Allocate/resize the cache so it can store indices up to n. */
static void ensure_cache(idx_t n)
{
    /* We always need room for at least 4 entries to hold the first terms. */
    idx_t needed = (n > 3) ? (n + 1) : 4;   /* +1 because indices start at 0 */
    if (needed <= cache_cap) return;

    idx_t new_cap = (cache_cap > 0) ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A025942. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialize the first four terms if not already done */
    if (cache_len < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)361;
        cache[3] = (seq_t)4907;
        cache_len = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence: a(i) = 23*a(i-1) - 168*a(i-2) + 468*a(i-3) - 432*a(i-4) */
        seq_t val = 23 * cache[i-1]
                    - 168 * cache[i-2]
                    + 468 * cache[i-3]
                    - 432 * cache[i-4];
        cache[i] = val;
    }

    if (n > cache_len) cache_len = n;
    return cache[n];
}
