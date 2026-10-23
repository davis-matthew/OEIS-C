/*
 * A116396  Expansion of 2/((2+x)*sqrt(1-4*x)-x).
 * https://oeis.org/A116396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (4^(n−k)/2^k) * * Sum_{j=0..k}
 *            C(k,j)·C(n−k+(j−1)/2,n−k)·(−1)^(k−j). * Recurrence (D‑finite): *
 *            2*n*a(n) + (−13*n+10)*a(n−1) + (9*n−16)*a(n−2) * +
 *            2*(19*n−41)*a(n−3) + (23*n−66)*a(n−4) + 2*(2*n−7)*a(n−5) = 0 *
 *            Initial values: a(0)=1, a(1)=2, a(2)=7, a(3)=25, a(4)=93
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n) {
    /* static cache – first five terms are known */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size == 0) {                 /* initialise cache */
#ifdef __cplusplus
        cache = new seq_t[5];
#else
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
#endif
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)93;
        cache_size = 5;
    }

    if (n < cache_size)                    /* already known */
        return cache[n];

    /* need to grow the cache to hold terms up to index n */
    idx_t needed = n + 1;
    idx_t old_size = cache_size;

    if (needed > cache_size) {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[needed];
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;        /* allocation failed – unlikely for n ≤ 10000 */
        cache = new_cache;
#endif
        cache_size = needed;
    }

    /* compute terms from old_size up to n (inclusive) */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t term1, term2, term3, term4, term5, numerator, denominator;

        /* a(i-1) */
        term1 = ((seq_t)(13 * i - 10)) * cache[i - 1];

        /* a(i-2) */
        term2 = ((seq_t)(9 * i - 16)) * cache[i - 2];

        /* a(i-3) */
        term3 = ((seq_t)(2 * (19 * i - 41))) * cache[i - 3];

        /* a(i-4) */
        term4 = ((seq_t)(23 * i - 66)) * cache[i - 4];

        /* a(i-5) */
        term5 = ((seq_t)(2 * (2 * i - 7))) * cache[i - 5];

        numerator = term1 - term2 - term3 - term4 - term5;
        denominator = (seq_t)(2 * i);           /* 2*i > 0 for i ≥ 5 */

        cache[i] = numerator / denominator;
    }

    return cache[n];
}
