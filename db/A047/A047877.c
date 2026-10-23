/*
 * A047877  a(n) = (1/2)*Sum_{i=0..n} T(i,n-i), array T as in A049604.
 * https://oeis.org/A047877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) + a(n-4) - a(n-6) - 2*a(n-7) + a(n-10) for n >
 *            14 Initial values: a(0)=0, a(1)=3, a(2)=4, a(3)=4, a(4)=6,
 *            a(5)=9, a(6)=11, a(7)=14, a(8)=18, a(9)=21, a(10)=26, a(11)=32,
 *            a(12)=34, a(13)=41, a(14)=49
 * Range:     u8 n <= 34, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* memoized values */
static size_t   cache_len  = 0; /* number of values stored (≤ cache_cap) */
static size_t   cache_cap  = 0; /* allocated capacity */

/* initialize memo table with the first 15 terms */
static void init_cache(void)
{
    const size_t init_cap = 16; /* at least 15 */
    cache_cap = init_cap;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);  /* out of memory */

    cache[0]  = (seq_t)0;
    cache[1]  = (seq_t)3;
    cache[2]  = (seq_t)4;
    cache[3]  = (seq_t)4;
    cache[4]  = (seq_t)6;
    cache[5]  = (seq_t)9;
    cache[6]  = (seq_t)11;
    cache[7]  = (seq_t)14;
    cache[8]  = (seq_t)18;
    cache[9]  = (seq_t)21;
    cache[10] = (seq_t)26;
    cache[11] = (seq_t)32;
    cache[12] = (seq_t)34;
    cache[13] = (seq_t)41;
    cache[14] = (seq_t)49;
    cache_len = 15;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* for fast small indices */
    if (n < 15) {
        if (!cache) init_cache();
        return cache[(size_t)n];
    }

    if (!cache) init_cache();

    /* Grow cache to hold up to n */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using recurrence */
    for (size_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-3) + a(i-4) - a(i-6) - 2*a(i-7) + a(i-10) */
        seq_t t1 = 2 * cache[i - 3] + cache[i - 4] + cache[i - 10];
        seq_t t2 =            cache[i - 6] + 2 * cache[i - 7];
        cache[i] = t1 - t2;                /* guarantees non‑negative result   */
    }
    cache_len = need;
    return cache[(size_t)n];
}
