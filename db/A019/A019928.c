/*
 * A019928  Expansion of 1/((1-5x)(1-7x)(1-8x)).
 * https://oeis.org/A019928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 131*a(n-2) + 280*a(n-3); a(0) = 1, a(1) = 20,
 *            a(2) = 269
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of sequence A019928, expansion of 1/((1-5x)(1-7x)(1-8x)). */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;     /* cached values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_size = 0;     /* number of values stored */

/* Function: a
 *   Compute a(n) for any n >= 0, caching intermediate results.
 *   The cache is grown only when needed.  The function is thread-unsafe
 *   but meets the required time constraints.
 */
seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;                 /* number of elements needed */
    if (cache == NULL) {                         /* first call */
        if (need < 3) need = 3;                  /* ensure room for base values */
        cache_cap = need;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        /* base values */
        cache[0] = 1;
        cache[1] = 20;
        cache[2] = 269;
        cache_size = 3;
    } else if (need > cache_cap) {               /* enlarge cache */
        size_t new_cap = need;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* fill cache up to n using the recurrence */
    if (cache_size <= (size_t)n) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)20;
            seq_t t2 = cache[i - 2] * (seq_t)131;
            seq_t t3 = cache[i - 3] * (seq_t)280;
            cache[i] = t1 - t2 + t3;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
