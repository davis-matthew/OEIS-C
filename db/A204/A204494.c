/*
 * A204494  Number of (n+2)X3 0..2 arrays with every 3X3 subblock having three
 *          strictly increasing elements in a row horizontally or vertically.
 * https://oeis.org/A204494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 26*a(n-2) + 677*a(n-3) + 1863*a(n-4) +
 *            3391*a(n-5) + 2222*a(n-6) + 485*a(n-7) - 1392*a(n-8) -
 *            864*a(n-9) - 324*a(n-10) Initial values: a(1)=3971, a(2)=46458,
 *            a(3)=699151, a(4)=6047817, a(5)=65606908, a(6)=797822969,
 *            a(7)=8067170432, a(8)=86968465645, a(9)=981144743357,
 *            a(10)=10426...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* initial values for n = 1 .. 10 */
    static const seq_t init_vals[11] = {
        0ULL,          /* unused, a(0) */
        3971ULL,
        46458ULL,
        699151ULL,
        6047817ULL,
        65606908ULL,
        797822969ULL,
        8067170432ULL,
        86968465645ULL,
        981144743357ULL,
        10426325449799ULL
    };

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                 /* start modestly */
        if (cache_cap < 11)
            cache_cap = 11;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                   /* placeholder for a(0) */
        for (size_t i = 1; i <= 10; ++i)
            cache[i] = init_vals[i];
        cache_max = 10;
    }

    /* basic bounds: we only care about n >= 1 */
    if (n == 0)
        return 0;

    /* if the value is already cached, return it */
    if (n <= cache_max)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t v = cache[i - 1];
        v += (seq_t)26   * cache[i - 2];
        v += (seq_t)677  * cache[i - 3];
        v += (seq_t)1863 * cache[i - 4];
        v += (seq_t)3391 * cache[i - 5];
        v += (seq_t)2222 * cache[i - 6];
        v += (seq_t)485  * cache[i - 7];
        v -= (seq_t)1392 * cache[i - 8];
        v -= (seq_t)864  * cache[i - 9];
        v -= (seq_t)324  * cache[i -10];
        cache[i] = v;
    }
    cache_max = n;
    return cache[n];
}
