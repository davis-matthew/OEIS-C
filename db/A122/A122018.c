/*
 * A122018  Modulo 2 recursion switch between A000898 and A121966: A000898
 *          first.
 * https://oeis.org/A122018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2 For n ≥ 2 : if n is odd : a(n) = a(n‑1) –
 *            (n‑1)·a(n‑2) if n is even: a(n) = 2·a(n‑1) + 2·(n‑1)·a(n‑2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..cache_size-1] already computed */
static idx_t cache_size = 0;  /* number of valid entries in cache */

seq_t a(idx_t n)
{
    /* Initialize cache for the first two terms if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(1);                /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    if (n < cache_size)                /* already computed */
        return cache[n];

    /* Expand cache array to hold up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;

    /* compute missing terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if ((i & 1) == 0) {            /* even i */
            seq_t part2 = (seq_t)2;
            part2 *= (seq_t)(i - 1);
            part2 *= cache[i - 2];
            cache[i] = (seq_t)2 * cache[i - 1] + part2;
        } else {                       /* odd i */
            cache[i] = cache[i - 1] - (seq_t)(i - 1) * cache[i - 2];
        }
    }
    cache_size = n + 1;                /* all terms up to n are now cached */
    return cache[n];
}
