/*
 * A137246  a(n) is the ratio of the sum of the squares of the bends
 *          (curvatures) of the n-th generation of an Apollonian packing to
 *          the sum of the squares of the bends of the initial four-circle
 *          configuration.
 * https://oeis.org/A137246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - 3*a(n-2) for n>=4, a(1) = 1, a(2) = 17, a(3)
 *            = 339
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array holding computed terms */
static idx_t computed_max = 0;       /* highest n that has been computed */
static size_t cache_cap = 0;         /* current allocated capacity */

/* grow the cache to at least n elements */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;                    /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)n)
        new_cap <<= 1;             /* double until sufficient */

    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)                 /* allocation failure */
        abort();                 /* terminate – simplification */

    if (cache) {
        memcpy(new_arr, cache,
               cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* return a(n) following the OEIS definition */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    ensure_capacity(n);

    if (n > computed_max) {       /* compute missing terms */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)1;
            else if (i == 2)
                val = (seq_t)17;
            else if (i == 3)
                val = (seq_t)339;
            else
                /* a(i) = 20*a(i-1) - 3*a(i-2) */
                val = cache[i-2] * (seq_t)20 - cache[i-3] * (seq_t)3;
            cache[i-1] = val;     /* store in cache, zero‑based */
        }
        computed_max = n;
    }

    return cache[n-1];
}
