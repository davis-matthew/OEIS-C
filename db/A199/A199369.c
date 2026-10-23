/*
 * A199369  Primes of the form 4*n^3 - 3.
 * https://oeis.org/A199369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 4*k^3-3 (searching over
 *            integer k)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 88, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for 0, return 0 */

    /* static cache of found primes and last tested k */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t  last_k = 0;    /* last value of k tested */

    /* if cache is empty, allocate initial capacity */
    if (cache == NULL) {
        /* start with capacity for 16 primes */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        /* test next candidate k */
        last_k++;
        idx_t k = last_k;
        seq_t kk = (seq_t)k;
        seq_t val = 4 * kk * kk * kk - 3;   /* 4*k^3 - 3 */

        if (isPrimeGeneric(val)) {
            /* ensure capacity for new element */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = val;
        }
    }

    return cache[n - 1];
}
