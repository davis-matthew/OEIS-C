/*
 * A228472  a(n) = 6*a(n-2) + a(n-4), where a(0) = 5, a(1) = 8, a(2) = 30,
 *          a(3) = 49.
 * https://oeis.org/A228472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) + a(n-4) initial values: a(0)=5, a(1)=8,
 *            a(2)=30, a(3)=49
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

/* static storage for memoisation ------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* currently allocated slots          */
static idx_t  cache_max  = 0;      /* highest index that has been computed */

/* initialise the cache with the four starting terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;                      /* already initialised */

    cache_cap = 4;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();

    cache[0] = (seq_t)5;
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)30;
    cache[3] = (seq_t)49;
    cache_max = 3;
}

/* grow the cache so that indices up to `req` are valid, compute missing terms */
static void ensure_capacity(idx_t req)
{
    /* extend allocation if necessary */
    if ((size_t)req + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while ((size_t)req + 1 > new_cap)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all terms that are still missing */
    if (req > cache_max) {
        for (idx_t i = cache_max + 1; i <= req; ++i) {
            /* recurrence uses a(i-2) and a(i-4), which already exist */
            cache[i] = (seq_t)6 * cache[i - 2] + cache[i - 4];
        }
        cache_max = req;
    }
}

seq_t a(idx_t n)
{
    init_cache();          /* guarantee base values are present */

    if (n <= 3)
        return cache[n];  /* direct access for the known terms */

    ensure_capacity(n);   /* compute all necessary terms */
    return cache[n];
}
