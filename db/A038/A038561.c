/*
 * A038561  Left-hand border of triangle A046937.
 * https://oeis.org/A038561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A038561 – Left-hand border of triangle A046937
 *
 *  Definition:
 *      a(0) = 1
 *      a(1) = 2
 *      a(n) = Σ_{k=0}^{n-1} C(n-1,k) · a(k)   for n ≥ 2
 *
 *  This function caches results so that calls with arbitrary arguments
 *  are fast.  All arithmetic uses the type `seq_t` supplied by the
 *  build system.  The implementation is portable between C11 and
 *  C++17 by guarding the `malloc`/`realloc` casts appropriately.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache         = NULL;   /* cache[0] … cache[computed_max] */
static size_t cache_size    = 0;      /* allocated length of cache array */
static idx_t   computed_max = 0;      /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Initialise the cache on first use. */
    if (!cache)
    {
        cache_size = 2;
#ifdef __cplusplus
        cache = static_cast<seq_t*>(malloc(cache_size * sizeof(seq_t)));
#else
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)2;  /* a(1) */
        computed_max = 1;
    }

    /* If the requested value is already cached, return it. */
    if (n <= computed_max) return cache[n];

    /* Ensure the cache is large enough. */
    if (n + 1 > cache_size)
    {
        size_t new_size = n + 1;
#ifdef __cplusplus
        seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all new terms up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;  /* C(i-1,0) */

        for (idx_t k = 0; k <= i - 1; ++k)
        {
            sum += cache[k] * binom;

            if (k < i - 1)
            {
                seq_t num = (seq_t)(i - 1 - k);
                seq_t den = (seq_t)(k + 1);
                binom = (binom * num) / den;
            }
        }
        cache[i] = sum;
    }

    computed_max = n;
    return cache[n];
}
