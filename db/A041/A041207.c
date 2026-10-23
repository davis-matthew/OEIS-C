/*
 * A041207  Denominators of continued fraction convergents to sqrt(114).
 * https://oeis.org/A041207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 2050*a(n-6) - a(n-12) Initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=31, a(4)=65, a(5)=96, a(6)=1985, a(7)=2081,
 *            a(8)=6147, a(9)=63551, a(10)=133249, a(11)=196800
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function definition as required */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* Initialisation: fill cache with the first 12 terms */
    if (cap == 0) {
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;   cache[1] = 1;   cache[2] = 3;
        cache[3] = 31;  cache[4] = 65;  cache[5] = 96;
        cache[6] = 1985; cache[7] = 2081; cache[8] = 6147;
        cache[9] = 63551; cache[10] = 133249; cache[11] = 196800;
    }

    /* If the requested n is already cached, just return it */
    if (n < cap)
        return cache[n];

    /* Grow the cache to accommodate up to n */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cap = new_cap;

    /* Compute missing values using the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        cache[i] = (seq_t)2050 * cache[i - 6] - cache[i - 12];
    }

    return cache[n];
}
