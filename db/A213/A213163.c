/*
 * A213163  Sequence of coefficients of x in marked mesh pattern generating
 *          function Q_{n,132}^(3,0,-,0)(x).
 * https://oeis.org/A213163
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(4)=1, a(5)=7, a(6)=32, a(7)=122, a(8)=422
   a(n) = 7*a(n-1) - 17*a(n-2) + 17*a(n-3) - 7*a(n-4) + a(n-5)  (n ≥ 9) */
#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* cache array: index i stores a(i)  (valid for i ≥ 4) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored so far (size) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first five values when first called */
    if (cache_len == 0) {
        cache_cap = 9;                 /* indices 0..8, we store up to 8 initially */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* values for indices 0..3 are irrelevant */
        cache[4] = (seq_t)1;           /* a(4) */
        cache[5] = (seq_t)7;           /* a(5) */
        cache[6] = (seq_t)32;          /* a(6) */
        cache[7] = (seq_t)122;         /* a(7) */
        cache[8] = (seq_t)422;         /* a(8) */
        cache_len = 9;                 /* next free index is 9 */
    }

    /* if n < 4 we return 0 – undefined but harmless */
    if (n < 4)
        return (seq_t)0;

    /* if the requested n is already in cache, return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache to contain indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)7;
        val += cache[i-3] * (seq_t)17;
        val += cache[i-5];
        val -= cache[i-2] * (seq_t)17;
        val -= cache[i-4] * (seq_t)7;
        cache[i] = val;
    }
    cache_len = n + 1;   /* update stored length */

    return cache[n];
}
