/*
 * A068907  Number of partitions of n modulo 3.
 * https://oeis.org/A068907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) mod 3 where p(n) is the partition number of n
 *            (Euler's recurrence with generalized pentagonal numbers). The
 *            static cache allows O(n √n) incremental computation
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *mod3_cache = NULL;   /* stores p(n) mod 3 for 0..size-1   */
static idx_t  cache_size  = 0;     /* number of computed terms           */
static idx_t  cache_cap   = 0;     /* capacity of the allocated array    */

/* Ensure the cache holds values up to and including idx n */
static void
ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    /* Increase capacity if needed */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n + 1) new_cap <<= 1;
        mod3_cache = (mod3_cache
                      ? (seq_t *)realloc(mod3_cache, new_cap * sizeof(seq_t))
                      : (seq_t *)malloc(new_cap * sizeof(seq_t)));
        cache_cap = new_cap;
    }

    /* Initialize p(0) = 1 if this is the first call */
    if (cache_size == 0) {
        mod3_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute new terms up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        int val = 0;
        for (idx_t k = 1; ; ++k) {
            /* Generalized pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;      /* k(3k-1)/2    */
            idx_t pent2 = pent1 + k;                /* k(3k+1)/2    */
            if (pent1 > m && pent2 > m) break;

            int sign = (k & 1) ? 1 : -1;    /* (-1)^{k+1} */

            if (m >= pent1) {
                val += sign * (int)mod3_cache[m - pent1];
            }
            if (m >= pent2) {
                val += sign * (int)mod3_cache[m - pent2];
            }
        }
        /* Reduce modulo 3 */
        val %= 3;
        if (val < 0) val += 3;
        mod3_cache[m] = (seq_t)val;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return mod3_cache[n];
}
