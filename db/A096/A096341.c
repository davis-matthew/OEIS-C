/*
 * A096341  E.g.f.: exp(x)/(1-x)^7.
 * https://oeis.org/A096341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+7)*a(n-1) – (n-1)*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */
    static idx_t cache_cap = 0;   /* allocation capacity */

    /* initialise cache with a(0) */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        cache_len = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;          /* a(0) = 1 */
        } else if (i == 1) {
            val = (seq_t)8;          /* a(1) = 8 */
        } else {
            seq_t term1 = ((seq_t)(i + 7)) * cache[i - 1];
            seq_t term2 = ((seq_t)(i - 1)) * cache[i - 2];
            val = term1 - term2;
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
