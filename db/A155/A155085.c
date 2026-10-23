/*
 * A155085  a(n) = n + sum of divisors of n.
 * https://oeis.org/A155085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + sigma(n) (sigma = sum of divisors of n)
 * Range:     u8 n <= 71, u16 n <= 9771, u32 n <= 9771, u64 n <= 9771,
 *            u128 n <= 9771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sum of divisors of n (n >= 1). */
static seq_t sum_of_divisors(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t d = n / i;
            if (d != i) sum += (seq_t)d;
        }
    }
    return sum;
}

/* Main function */
seq_t a(idx_t n) {
    static seq_t *sigma_cache = NULL;  /* cache of sigma values */
    static idx_t cache_size = 0;       /* current allocated size */

    if (n >= cache_size) {
        /* extend cache to hold sigma(k) for k = 1..n */
        size_t new_size = (size_t)(n + 1);                        /* include index 0 */
        seq_t *new_cache = (seq_t *)realloc(sigma_cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* fallback on OOM, not expected */
        sigma_cache = new_cache;

        /* compute new sigma values */
        for (idx_t k = cache_size; k <= n; ++k) {
            sigma_cache[k] = sum_of_divisors(k);
        }
        cache_size = n + 1;
    }

    /* a(n) = n + sigma(n) */
    return sigma_cache[n] + (seq_t)n;
}
