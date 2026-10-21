/*
 * A052502  Number of permutations sigma of [3n] without fixed points such
 *          that sigma^3 = Id.
 * https://oeis.org/A052502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n)!/(3^n * n!) = (3n-1)*(3n-2)*a(n-1), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to handle non‑increasing calls efficiently */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of values already computed */

    /* Initialise cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;      /* out of memory: return zero */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t mult = ((seq_t)(3 * i - 1)) * ((seq_t)(3 * i - 2));
            cache[i] = mult * cache[i - 1];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
