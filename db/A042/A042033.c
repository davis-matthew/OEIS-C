/*
 * A042033  Denominators of continued fraction convergents to sqrt(540).
 * https://oeis.org/A042033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 238142*a(n-8) - a(n-16) (for n ≥ 16) * Initial values:
 *            a(0)=1, a(1)=4, a(2)=17, a(3)=21, a(4)=227, a(5)=248, *
 *            a(6)=1219, a(7)=5124, a(8)=236923, a(9)=952816, * a(10)=4048187,
 *            a(11)=5001003, a(12)=54058217, * a(13)=59059220,
 *            a(14)=290295097, a(15)=1220239608
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;   /* array of computed terms */
static idx_t  cache_len = 0; /* number of elements currently stored */
static idx_t  cache_cap = 0; /* allocated capacity of the array */

/* seed values for n = 0 … 15 */
static const seq_t init_vals[16] = {
    (seq_t)1, (seq_t)4, (seq_t)17, (seq_t)21,
    (seq_t)227, (seq_t)248, (seq_t)1219, (seq_t)5124,
    (seq_t)236923, (seq_t)952816, (seq_t)4048187, (seq_t)5001003,
    (seq_t)54058217, (seq_t)59059220, (seq_t)290295097, (seq_t)1220239608
};

/* ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* First call – initialise cache with the 16 seed values */
    if (cache == NULL) {
        /* Allocate at least 16 slots, enough for the initial values.
           If n < 15 we still allocate the full 16 to keep the code simple. */
        cache_cap = (n + 1 > 16) ? n + 1 : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                 /* memory allocation failure */
        for (idx_t i = 0; i < 16; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 16;
    }
    else if (n + 1 > cache_cap) {
        /* Grow the cache if necessary */
        cache_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;                 /* memory allocation failure */
        cache = tmp;
    }

    /* if n already in cache, nothing more to do */
    if (n < cache_len) return;

    /* compute missing values, starting after the last known index */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = ((seq_t)238142) * cache[i - 8] - cache[i - 16];
    }
    cache_len = n + 1;
}

/* externally visible function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len) return cache[n];
    return (seq_t)0;   /* should never be reached, but silences compiler warnings */
}
