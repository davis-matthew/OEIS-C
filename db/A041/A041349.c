/*
 * A041349  Denominators of continued fraction convergents to sqrt(188).
 * https://oeis.org/A041349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9214 * a(n-8) - a(n-16)  for n > 15.
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=45, a(5)=97, a(6)=239,
 *   a(7)=336, a(8)=8975, a(9)=9311, a(10)=27597, a(11)=64505,
 *   a(12)=414627, a(13)=893759, a(14)=2202145, a(15)=3095904.
 */
#include <stdlib.h>
#include <string.h>   /* for memset */

/* Pre‑computed initial 16 terms */
static const seq_t base_terms[16] = {
    1ULL, 1ULL, 3ULL, 7ULL, 45ULL, 97ULL, 239ULL, 336ULL,
    8975ULL, 9311ULL, 27597ULL, 64505ULL, 414627ULL,
    893759ULL, 2202145ULL, 3095904ULL
};

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;   /* pointer to cache array */
    static size_t cache_cap = 0;  /* allocated size of cache array   */
    static idx_t  cache_len = 0;  /* highest index that has been computed (inclusive) */

    /* Ensure the cache is large enough for indices 0..n */
    if (cache == NULL) {
        /* First allocation: use calloc to zero the memory */
        cache = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache_cap = (size_t)n + 1;
    } else if ((size_t)(n + 1) > cache_cap) {
        /* Grow the cache to accommodate up to index n */
        size_t old_cap = cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        /* Zero the newly added part of the array */
        if (cache_cap < (size_t)(n + 1)) {
            memset(cache + old_cap,
                   0,
                   ((size_t)n + 1 - old_cap) * sizeof(seq_t));
        }
        cache_cap = (size_t)n + 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 16) {
            /* Base term */
            cache[i] = base_terms[i];
        } else {
            /* Recurrence: a(n) = 9214*a(n-8) - a(n-16) */
            cache[i] = (seq_t)9214 * cache[i - 8] - cache[i - 16];
        }
    }

    /* Update the stored maximum computed index */
    if (n + 1 > cache_len) {
        cache_len = n + 1;
    }

    return cache[n];
}
