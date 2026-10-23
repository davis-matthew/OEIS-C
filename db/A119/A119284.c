/*
 * A119284  Alternating sum of the cubes of the first n Fibonacci numbers.
 * https://oeis.org/A119284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) + 2 a(n-1) - 9 a(n-2) + 3 a(n-3) + 4 a(n-4) - a(n-5) = 0
 *            Initial values: a(0)=0, a(1)=-1, a(2)=0, a(3)=-8, a(4)=19
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 32, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array          */
static idx_t cache_capacity = 0;     /* current allocated size     */
static idx_t cache_max = (idx_t)-1;  /* highest index computed     */

static const seq_t init_vals[5] = { (seq_t)0, (seq_t)-1, (seq_t)0, (seq_t)-8, (seq_t)19 };

/* ensure cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* allocate or grow storage if necessary */
    if (!cache) {
        idx_t sz = n + 1;
        if (sz < 5) sz = 5;
        cache = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!cache) exit(1);
        /* copy initial values */
        for (idx_t i = 0; i < 5; ++i) cache[i] = init_vals[i];
        cache_capacity = sz;
        cache_max = 4;
    } else if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 5;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing values */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i < 5) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = -(seq_t)2 * cache[i-1]
                           + (seq_t)9 * cache[i-2]
                           - (seq_t)3 * cache[i-3]
                           - (seq_t)4 * cache[i-4]
                           + cache[i-5];
            }
        }
        cache_max = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
