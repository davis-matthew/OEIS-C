/*
 * A206199  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-3) - 5*a(n-4) + 2*a(n-5) for n > 6
 *            Initial values: a(1) = 256 a(2) = 480 a(3) = 1768 a(4) = 7388
 *            a(5) = 29724 a(6) = 117084
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache of computed terms; index 0 is unused */
static seq_t *cache = NULL;          /* array of a(i) for i >= 0 */
static size_t cache_size = 0;        /* allocated size of cache */
static idx_t last_computed = 0;      /* last index fully computed */

/* ensure cache can hold at least up to index n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (n <= last_computed)
        return;

    /* resize array if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = (cache_size == 0 ? 10 : cache_size * 2);
        while (new_size <= (size_t)n)
            new_size *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL)
            return;   /* allocation failure – caller will get garbage */
        if (cache != NULL) {
            memcpy(new_cache, cache, sizeof(seq_t) * (last_computed + 1));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = 256;
        else if (i == 2)
            cache[2] = 480;
        else if (i == 3)
            cache[3] = 1768;
        else if (i == 4)
            cache[4] = 7388;
        else if (i == 5)
            cache[5] = 29724;
        else if (i == 6)
            cache[6] = 117084;
        else
            cache[i] = 3 * cache[i-1] + 17 * cache[i-3]
                       - 5 * cache[i-4] + 2 * cache[i-5];
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not part of the sequence, but safe fallback */

    ensure_up_to(n);
    return cache[n];
}
