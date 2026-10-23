/*
 * A189101  Expansion of g.f. 1/(1-(x+x^2+x^3+x^4+x^6+x^7)).
 * https://oeis.org/A189101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-6)+a(n-7) with a(0)=1 and
 *            a(k)=0 for k<0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Initial allocation or resizing */
    if (cache_size == 0) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        max_computed = 0;
    } else if (n >= cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Return immediately if already computed */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 1) sum += cache[i - 1];
        if (i >= 2) sum += cache[i - 2];
        if (i >= 3) sum += cache[i - 3];
        if (i >= 4) sum += cache[i - 4];
        if (i >= 6) sum += cache[i - 6];
        if (i >= 7) sum += cache[i - 7];
        cache[i] = sum;
    }
    max_computed = n;
    return cache[n];
}
