/*
 * A053532  Expansion of e.g.f.: (1-x)^(-1/2)*exp(-x/2 -x^2/4 -x^3/6).
 * https://oeis.org/A053532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  a(1)=a(2)=a(3)=0
   For n >= 4:
   a(n) = (n-1)*a(n-1) + ((n-1)*(n-2)*(n-3)/2)*a(n-4)   */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* helper to grow the cache to at least 'needed' elements */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure *//* pragma -space */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1;
        } else if (k <= 3) {
            val = (seq_t)0;
        } else {
            seq_t term1 = (seq_t)(k - 1) * cache[k - 1];
            seq_t coeff = (seq_t)(k - 1) * (k - 2);
            coeff = coeff * (k - 3);
            coeff = coeff / (seq_t)2;               /* exact division */
            seq_t term2 = coeff * cache[k - 4];
            val = term1 + term2;
        }
        cache[k] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
