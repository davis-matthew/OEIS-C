/*
 * A120771  Expansion of ( 1-x^3+x^4+x^5-x^8 ) / ( 1-2*x^3-x^6+x^9 ).
 * https://oeis.org/A120771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + a(n-6) - a(n-9) Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=3, a(7)=2, a(8)=1
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;      /* array of a(i) */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* current capacity */

    /* first 9 terms, defined by the recurrence */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)3, (seq_t)2, (seq_t)1
    };

    /* initialise cache with first 9 terms on first entry */
    if (cache == NULL) {
        cache_cap = 9;
        cache_len = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
    }

    /* if the requested index is already computed, just return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* need to extend cache up to index n */
    size_t target = (size_t)n;
    if (target >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= target)
            new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed; abort with 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= target; ++i) {
        cache[i] = (seq_t)2 * cache[i-3] + cache[i-6] - cache[i-9];
    }
    cache_len = target + 1;

    return cache[target];
}
