/*
 * A081069  a(n) = Lucas(4n)+2 = Lucas(2n)^2.
 * https://oeis.org/A081069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n)+2 = Lucas(2n)^2 Initialized by a(0)=4, a(1)=9.
 *            For n>1: a(n) = 7*a(n-1) - a(n-2) - 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated capacity */
    static idx_t cache_len  = 0;   /* number of values stored */

    /* enlarge cache if needed */
    if (n >= cache_size) {
        idx_t new_size = (n + 1 > 2 * cache_size) ? n + 1 : 2 * cache_size;
        if (new_size == 0) new_size = 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;      /* a(0) */
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)9;      /* a(1) */
        cache_len = 2;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)         val = (seq_t)4;
        else if (i == 1)    val = (seq_t)9;
        else                val = 7 * cache[i-1] - cache[i-2] - (seq_t)10;
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
