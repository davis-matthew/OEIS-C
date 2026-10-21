/*
 * A099025  Expansion of 1 / ((1+x) * (1-5*x+x^2)).
 * https://oeis.org/A099025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + (-1)^n, with a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamically sized cache */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;   /* number of terms currently stored */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* initialise cache at first call */
    if (cache == NULL) {
        cache_cap = 16;                               /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* guard against allocation failure – if it fails we simply return 0 */
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* return early if already computed */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if needed */
    if ((idx_t)n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= (idx_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i - 1];          /* a(i-1) */
        val = val * (seq_t)5;              /* 5*a(i-1) */
        val = val - cache[i - 2];          /* -a(i-2) */
        if ((i & 1) == 0)                  /* even i : +1  */
            val = val + (seq_t)1;
        else                               /* odd i  : -1  */
            val = val - (seq_t)1;
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
