/*
 * A042019  Denominators of continued fraction convergents to sqrt(533).
 * https://oeis.org/A042019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12236*a(n-5) + a(n-10) for n > 9 Initial values: a(0) =
 *            1, a(1) = 11, a(2) = 12, a(3) = 23, a(4) = 265, a(5) = 12213,
 *            a(6) = 134608, a(7) = 146821, a(8) = 281429, a(9) = 3242540
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated capacity */
    static idx_t cache_len_used = 0;  /* first unused index */

    /* Ensure the cache is large enough for index n+1 */
    idx_t needed = n + 1;
    if (cache == NULL || cache_size < needed) {
        idx_t new_size = needed;
        /* Allocate space for the base terms on the first expansion */
        if (new_size < 10) new_size = 10;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize the first ten terms once */
    static int base_inited = 0;
    if (!base_inited) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)23;
        cache[4] = (seq_t)265;
        cache[5] = (seq_t)12213;
        cache[6] = (seq_t)134608;
        cache[7] = (seq_t)146821;
        cache[8] = (seq_t)281429;
        cache[9] = (seq_t)3242540;
        cache_len_used = 10;
        base_inited = 1;
    }

    /* If n is already in the cache, return it directly */
    if (n < cache_len_used)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len_used; i <= n; ++i) {
        cache[i] = (seq_t)12236 * cache[i - 5] + cache[i - 10];
    }
    cache_len_used = n + 1;

    return cache[n];
}
