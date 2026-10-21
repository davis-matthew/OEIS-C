/*
 * A055814  Expansion of e.g.f.: exp(x^3/3 + x^2/2).
 * https://oeis.org/A055814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-2) + (n-1)*(n-2)*a(n-3); a(0)=1, a(1)=0,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache; grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge the cache to hold indices up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    cache = tmp;

    /* compute all new entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 0;
        } else if (i == 2) {
            cache[2] = 1;
        } else {
            seq_t term1 = ((seq_t)(i - 1)) * cache[i - 2];
            seq_t term2 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * cache[i - 3];
            cache[i] = term1 + term2;
        }
    }
    cache_len = new_len;

    return cache[n];
}
