/*
 * A077922  Expansion of (1-x)^(-1)/(1+2*x-x^2+x^3).
 * https://oeis.org/A077922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A077922
 *
 * Generating function: (1−x)^{-1}/(1+2x−x^2+x^3)
 * Recurrence (derived from the denominator):
 *
 *   a(0) = 1 , a(1) = –1 , a(2) = 4 , a(3) = –9
 *   for n ≥ 4:
 *     a(n) = –a(n−1) + 3·a(n−2) – 2·a(n−3) + a(n−4)
 *
 * The values are computed iteratively with memoisation so that
 * calls may arrive in any order.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache      = NULL;   /* pointer to cached values */
static idx_t cache_cap   = 0;      /* allocated capacity in elements */
static idx_t cache_size  = 0;      /* number of values currently stored */

/* initialise the cache with the first four terms */
static void init_cache(void)
{
    if (cache == NULL)
    {
        /* start with a small capacity and grow as needed */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return; /* allocation failure – unlikely in tests */

        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)-1;      /* a(1) */
        cache[2] = (seq_t)4;       /* a(2) */
        cache[3] = (seq_t)-9;      /* a(3) */
        cache_size = 4;
    }
}

/* ensure the cache can hold at least 'n+1' terms */
static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;

    /* double the capacity until it is sufficient */
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (ptr) {
        cache = ptr;
        cache_cap = new_cap;
    }
}

/* compute and memoise successive terms up to index n */
static void compute_up_to(idx_t n)
{
    if (n < cache_size) return;

    grow_cache(n);

    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t v =  -cache[i-1]
                 + (seq_t)3 * cache[i-2]
                 - (seq_t)2 * cache[i-3]
                 +       cache[i-4];
        cache[i] = v;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    init_cache();

    /* compute required terms */
    compute_up_to(n);

    /* return the cached value */
    return cache[n];
}
