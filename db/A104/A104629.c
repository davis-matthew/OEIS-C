/*
 * A104629  Expansion of (1-2*x-sqrt(1-4*x))/(x^2 * (1+2*x+sqrt(1-4*x))).
 * https://oeis.org/A104629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000957(n+3) Recurrence (n ≥ 2): 2*(n+3)*a(n) =
 *            (7*n+9)*a(n-1) + 2*(2*n+3)*a(n-2) Initial values: a(0)=1,
 *            a(1)=2, a(2)=6 This implementation caches computed values for
 *            O(n) time per new call
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoization */
static seq_t *cache = NULL;          /* array of computed a(n) values */
static idx_t cache_len = 0;          /* number of initialized entries in cache */
static idx_t cache_cap = 0;          /* allocated capacity of cache */

/* grow cache to accommodate at least 'need' entries */
static void ensure_cache(idx_t need) {
    if (need <= cache_cap)
        return;
    idx_t newcap = cache_cap ? cache_cap << 1 : 4;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) { /* allocation failed; terminate */
        abort();
    }
    cache = tmp;
    cache_cap = newcap;
}

/* public sequence function */
seq_t a(idx_t n) {
    /* if already computed, return cached value */
    if (n < cache_len)
        return cache[n];

    /* compute new entries up to n */
    ensure_cache(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)6;
        } else {
            /* a(i) = ((7*i+9)*a(i-1) + 2*(2*i+3)*a(i-2)) / (2*(i+3)) */
            seq_t term1 = (seq_t)((7 * i + 9) * cache[i - 1]);
            seq_t term2 = (seq_t)(2 * (2 * i + 3) * cache[i - 2]);
            seq_t numerator = term1 + term2;
            seq_t denominator = (seq_t)(2 * (i + 3));
            cache[i] = numerator / denominator;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
