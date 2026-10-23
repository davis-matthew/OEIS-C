/*
 * A025864  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A025864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)-a(n-9)+a(n-12)-a(n-16)-a(n-17)+a(n-21)
 *            Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    /* initial allocation for the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
    }

    /* if the index is already cached, just return it */
    if (n < cache_size)
        return cache[n];

    /* grow the cache so that it contains index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;         /* allocation failure */
    cache = new_cache;

    /* compute all new terms between the old size and n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 4)  val += cache[i - 4];
        if (i >= 5)  val += cache[i - 5];
        if (i >= 9)  val -= cache[i - 9];
        if (i >= 12) val += cache[i - 12];
        if (i >= 16) val -= cache[i - 16];
        if (i >= 17) val -= cache[i - 17];
        if (i >= 21) val += cache[i - 21];
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
