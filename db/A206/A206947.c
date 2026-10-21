/*
 * A206947  Number of nonisomorphic graded posets with 0 and non-uniform Hasse
 *          graph of rank n, with exactly 2 elements of each rank above 0.
 * https://oeis.org/A206947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) for n ≥ 5
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=14
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: terms 0 .. cache_len-1 have been computed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* if already computed, return immediately */
    if (cache && (size_t)n < cache_len)
        return cache[(size_t)n];

    /* initial allocation if necessary */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
    }

    /* grow cache capacity if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0 || i == 1 || i == 2)
            val = 0;
        else if (i == 3)
            val = 2;
        else if (i == 4)
            val = 14;
        else {
            /* recurrence: 8*a(i-1) - 21*a(i-2) + 20*a(i-3) - 5*a(i-4) */
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            val = 8 * t1 - 21 * t2 + 20 * t3 - 5 * t4;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
