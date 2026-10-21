/*
 * A052903  Expansion of (1-x^3)/(1-2x-x^3+x^4).
 * https://oeis.org/A052903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A052903: Expansion of (1-x^3)/(1-2x-x^3+x^4)
 * Formula: a(n) = a(n-3) + 2*a(n-1) - a(n-4)   (n ≥ 4)
 * Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached terms, initially empty */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first four terms */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache_len = 4;
    }

    /* if the requested n is already computed, simply return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache up to n */
    while (cache_len <= n) {
        /* grow storage if necessary */
        if ((size_t)cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap << 1 : 8;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        idx_t i = cache_len;   /* index of the new term */

        /* recurrence: a(i) = a(i-3) + 2*a(i-1) - a(i-4) */
        seq_t term = cache[i-3] + (seq_t)2 * cache[i-1] - cache[i-4];
        cache[i] = term;
        ++cache_len;
    }

    return cache[n];
}
