/*
 * A212905  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          |w-x|+|x-y+|y-z|=2n.
 * https://oeis.org/A212905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-4*a(n-3)+a(n-4)+2*a(n-5)-a(n-6) initial
 *            values: a(0)=1, a(1)=6, a(2)=16, a(3)=26, a(4)=42, a(5)=58,
 *            a(6)=82
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_N 20000   /* maximum n expected in the test harness */

seq_t a(idx_t n)
{
    /* static cache: first 20001 terms stored after initialisation */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_max  = (size_t)-1;  /* largest index already computed */

    /* initialise the cache on first entry */
    if (!cache) {
        cache_size = MAX_N + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* abort‑safe: return 0 on allocation failure */
    }

    /* initialise base values */
    if (cache_max == (size_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)42;
        cache[5] = (seq_t)58;
        cache[6] = (seq_t)82;
        cache_max = 6;
    }

    /* if the request is already computed, return it */
    if ((size_t)n <= cache_max)
        return cache[n];

    /* grow the allocated buffer if necessary (should not happen for n<=MAX_N) */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        if (new_size > MAX_N + 1)
            new_size = MAX_N + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms using the linear recurrence */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];

        /* compute 2*a1 + a2 - 4*a3 + a4 + 2*a5 - a6 in a safe order */
        seq_t temp = (seq_t)2 * a1 + a2 + a4 + (seq_t)2 * a5;
        temp -= (seq_t)4 * a3;
        temp -= a6;
        cache[i] = temp;
    }
    cache_max = (size_t)n;
    return cache[n];
}
