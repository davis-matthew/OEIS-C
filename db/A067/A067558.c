/*
 * A067558  Sum of squares of proper divisors of n.
 * https://oeis.org/A067558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d<n} d^2 = sigma_2(n) - n^2
 * Range:     u8 n <= 23, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"  // build_spf(idx_t limit, idx_t **spf_out)

static seq_t *cache = NULL;         /* stored seq_t values for indices 1..len */
static size_t cache_cap = 0;        /* allocated capacity of cache */
static size_t cache_len = 0;        /* number of values currently stored */
static uint64_t *spf = NULL;        /* smallest prime factor array */
static size_t spf_cap = 0;          /* allocated limit for spf array */

static void ensure_cache(size_t n) {
    if (n <= cache_len) return;  /* already computed */

    /* extend SPF array if necessary */
    if (spf_cap < n) {
        uint64_t *new_spf = NULL;
        if (spf) free(spf);
        build_spf(n, &new_spf);   /* new_spf[0..n] contains smallest prime factors */
        spf = new_spf;
        spf_cap = n;
    }

    /* extend cache array */
    if (cache_cap < n) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cache_cap = n;
    }

    /* compute missing values */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        idx_t tmp = k;
        seq_t sigma = 1;                     /* product of contributions */
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }

            seq_t term = 1;                  /* sum_{i=0..e} p^{2i} */
            seq_t p2   = (seq_t)p * (seq_t)p;
            seq_t power = p2;
            for (idx_t i = 1; i <= e; ++i) {
                term += power;
                power *= p2;
            }
            sigma *= term;
        }
        seq_t knorm = (seq_t)k;
        cache[k - 1] = sigma - knorm * knorm;  /* a(k) = sigma_2(k) - k^2 */
    }
    cache_len = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined but safe */
    ensure_cache(n);
    return cache[n - 1];
}
