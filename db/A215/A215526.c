/*
 * A215526  a(0)=a(1)=1, a(n) = a(n-2) + a(a(n-1) mod n).
 * https://oeis.org/A215526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; a(n) = a(n-2) + a(a(n-1) mod n)
 * Range:     u8 n <= 30, u16 n <= 84, u32 n <= 308, u64 n <= 1153,
 *            u128 n <= 4636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed terms */
static seq_t *cache = NULL;          /* array of values            */
static unsigned char *init = NULL;   /* 0 = uninitialized, 1 = init*/
static size_t cap = 0;               /* allocated capacity         */

/* enlarge allocation so that index n is addressable */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cap <= n) {
        size_t newcap = cap ? cap * 2 : 16;
        while ((idx_t)newcap <= n) newcap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        unsigned char *new_init = (unsigned char *)realloc(init,
                                                          newcap * sizeof(unsigned char));
        if (!new_cache || !new_init) abort();
        cache = new_cache;
        init = new_init;
        /* initialise new slots as uninitialised */
        for (size_t i = cap; i < newcap; ++i) init[i] = 0;
        cap = newcap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* if already computed, return it */
    if (init[n]) return cache[n];

    /* compute all missing terms up to n (in increasing order) */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (init[i]) continue;
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
            init[i] = 1;
        } else {
            idx_t j = (idx_t)(cache[i-1] % i);
            cache[i] = cache[i-2] + cache[j];
            init[i] = 1;
        }
    }
    return cache[(size_t)n];
}
