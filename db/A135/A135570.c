/*
 * A135570  a(n) = 1 + Sum_{i=1..n} S2(i)*2^i, where S2(n) is digit sum of n,
 *          n in binary notation.
 * https://oeis.org/A135570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 1 + Sum_{i=1..n} S2(i)*2^i, where S2(i) is the binary
 *            digit sum of i. The function keeps a cache so that any order of
 *            calls is handled efficiently
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cache of already computed values */
    static seq_t *cache = NULL;  /* array of a(0) ... a(current_n)   */
    static idx_t cache_size = 0; /* number of elements allocated    */
    static idx_t current_n = 0;  /* largest n that has been computed */
    static seq_t sum = 0;        /* Σ_{i=1..current_n} popcount(i)*2^i */
    static seq_t power = 1;      /* 2^i during extension             */

    /* initialise on first call: a(0) = 1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache_size = 1;
        sum = (seq_t)0;
        power = (seq_t)1;
        current_n = 0;
    }

    /* extend the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;

        /* compute new values from current_n+1 .. n */
        for (idx_t i = current_n + 1; i <= n; ++i) {
            power <<= 1;                                 /* power = 2^i */
            unsigned pop = __builtin_popcountll((unsigned long long)i);
            if (pop != 0) {
                sum += power * (seq_t)pop;                /* add pop*2^i */
            }
            cache[i] = sum + (seq_t)1;                   /* a(i) = 1 + sum */
        }
        current_n = n;
    }

    return cache[n];
}
