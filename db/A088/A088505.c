/*
 * A088505  a(n) = (2^(3*n-1))/(integral_{x=0..1} (1-x^4)^n dx).
 * https://oeis.org/A088505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5; Recurrence: for n≥2, a(n) = 2*(4*n+1)/n * a(n-1) (exact
 *            integer division)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache for forward computation */
    static idx_t max_computed = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) return 0; /* outside offset, not used */

    /* Ensure enough space */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* realloc works even when cache is NULL */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 5;
        } else {
            /* recurrence factor: 2*(4*i+1)/i */
            seq_t numer = 2 * ( (seq_t)4 * i + 1 );
            seq_t denom = (seq_t)i;
            seq_t g1 = gcd(numer, denom);
            numer /= g1;
            denom /= g1;

            seq_t a_prev = cache[i - 1];
            seq_t g2 = gcd(a_prev, denom);
            a_prev /= g2;
            denom /= g2; /* denom should now be 1 */

            cache[i] = a_prev * numer;
        }
    }

    max_computed = ((max_computed > n) ? max_computed : n);
    return cache[n];
}
