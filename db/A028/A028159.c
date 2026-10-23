/*
 * A028159  Expansion of 1/((1-4*x)*(1-8*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A028159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028159: Expansion of 1/((1-4*x)*(1-8*x)*(1-10*x)*(1-12*x)).
   Recurrence: a(0)=1, a(1)=34, a(2)=740, a(3)=13160,
   a(n)=34*a(n-1)-416*a(n-2)+2144*a(n-3)-3840*a(n-4) */
#include <stdlib.h>

static seq_t *cache = NULL;     /* stored values a(0)…a(k) */
static size_t cache_len = 0;    /* number of values currently in cache */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* ensure cache has space for a(0)…a(upto) and compute missing terms */
static void ensure_cache(idx_t upto)
{
    if (upto < cache_len) return;            /* already computed */

    /* allocate / grow cache */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }
    while (cache_cap <= (size_t)upto) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= upto; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 34;
        } else if (i == 2) {
            cache[i] = 740;
        } else if (i == 3) {
            cache[i] = 13160;
        } else {
            cache[i] = 34 * cache[i-1]            /* 34*a(n-1) */
                     - 416 * cache[i-2]           /*  -416*a(n-2) */
                     + 2144 * cache[i-3]          /* +2144*a(n-3) */
                     - 3840 * cache[i-4];         /* -3840*a(n-4) */
        }
    }
    cache_len = upto + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
