/*
 * A201368  E.g.f.: 4*exp(4*x) / (5 - exp(4*x)).
 * https://oeis.org/A201368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-4)^(n-k) * 5^k * Stirling2(n,k) * k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence: a_m = 4^m + Σ_{i=0}^{m-1} C(m,i) * a_i * 4^{m-i-1} */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values and powers of 4 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated capacity */
    static idx_t cache_used = 0;   /* number of values stored (next index to compute) */

    static seq_t *pow4 = NULL;
    static idx_t pow4_size = 0;    /* allocated capacity */
    static idx_t pow4_used = 0;    /* number of powers stored (next index to compute) */

    /* Helper to ensure arrays have at least required capacity */
    if (cache_size < n + 1) {
        idx_t new_cap = cache_size ? cache_size * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_size = new_cap;
    }
    if (pow4_size < n + 1) {
        idx_t new_cap = pow4_size ? pow4_size * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        pow4 = (seq_t*)realloc(pow4, new_cap * sizeof(seq_t));
        pow4_size = new_cap;
    }

    /* Initialise first power of 4 */
    if (pow4_used == 0) {
        pow4[0] = (seq_t)1;
        pow4_used = 1;
    }

    /* Build missing powers of 4 up to n */
    for (idx_t i = pow4_used; i <= n; ++i) {
        pow4[i] = pow4[i - 1] * (seq_t)4;
    }
    pow4_used = n + 1;

    /* Initialise a(0) */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        cache_used = 1;
    }

    /* Compute values from cache_used up to n */
    for (idx_t m = cache_used; m <= n; ++m) {
        seq_t sum = 0;
        seq_t c   = 1;                 /* c will hold C(m,i) */
        for (idx_t i = 0; i < m; ++i) {
            /* term = C(m,i) * a[i] * 4^{m-i-1} */
            seq_t term = c * cache[i];
            term *= pow4[m - i - 1];
            sum += term;

            /* update C(m,i+1) = C(m,i) * (m-i) / (i+1) */
            c *= (seq_t)(m - i);
            c /= (seq_t)(i + 1);
        }
        cache[m] = pow4[m] + sum;   /* a_m = 4^m + sum */
    }
    cache_used = n + 1;

    return cache[n];
}
