/*
 * A163145  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 21 a(2) = 420 a(3) = 8400 a(4) = 19*a(3) +
 *            19*a(2) + 19*a(1) - 190*a(0) + 1 For n >= 5: a(n) = 19*a(n-1) +
 *            19*a(n-2) + 19*a(n-3) - 190*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache to avoid recomputation */
    static seq_t *cache = NULL;       /* array of a(0)..a(n) */
    static idx_t cache_len = 0;      /* number of already filled terms (cache_len==0 => none) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* ensure we have enough space */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? (n + 1) : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if n already computed, just return it */
    if (n < cache_len) return cache[n];

    /* compute missing terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)21;
        } else if (i == 2) {
            val = (seq_t)420;
        } else if (i == 3) {
            val = (seq_t)8400;
        } else if (i == 4) {
            /* a(4) uses the numerator constant +1 */
            val  = (seq_t)19 * cache[3];
            val += (seq_t)19 * cache[2];
            val += (seq_t)19 * cache[1];
            val -= (seq_t)190 * cache[0];
            val += (seq_t)1;
        } else {
            /* recurrence for n >= 5 */
            seq_t temp = (seq_t)19 * cache[i-1];
            temp += (seq_t)19 * cache[i-2];
            temp += (seq_t)19 * cache[i-3];
            val = temp - (seq_t)190 * cache[i-4];
        }
        cache[i] = val;
    }

    cache_len = n + 1;   /* now we have terms 0..n filled */
    return cache[n];
}
