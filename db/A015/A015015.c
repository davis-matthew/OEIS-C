/*
 * A015015  q-factorial numbers for q=-3.
 * https://oeis.org/A015015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)*((-3)^n - 1)/(-4) with a(0) = 1, a(1) = 1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Cached values of the sequence for memoisation.  */
static seq_t *cache      = NULL;  /* array of a(n) values          */
static size_t cache_cap  = 0;     /* allocated capacity            */
static idx_t  last_comp  = 0;     /* highest index computed so far */

/* Current power of (-3).  */
static seq_t pow_cur = (seq_t)(-3);

/* Initialise the cache and auxiliary data once.  */
static void init_cache(void) {
    if (cache == NULL) {
        /* Allocate at least two slots for a(0) and a(1).  */
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);              /* unlikely, just fail loudly */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_comp = 1;                    /* a(0) and a(1) are known */
    }
}

seq_t a(idx_t n) {
    /* Ensure the cache has the required space.  */
    if (cache == NULL) {
        init_cache();
    }
    size_t new_cap = (size_t)n + 1;
    if (new_cap < cache_cap) new_cap = cache_cap;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* unlikely, abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Base cases are already in the cache.  */
    if (n <= last_comp) return cache[n];

    /* Compute missing terms up to index n.  */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        /* pow_cur is currently (-3)^(i-1).  Multiply to obtain (-3)^i.  */
        pow_cur *= (seq_t)(-3);

        /* a(i) = ((-3)^i - 1) * a(i-1) / (-4).  */
        seq_t numerator = pow_cur - (seq_t)1;
        seq_t temp      = numerator * cache[i - 1];
        cache[i] = temp / (seq_t)(-4);
    }
    last_comp = n;
    return cache[n];
}
