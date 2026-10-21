/*
 * A233864  a(n) = |{0 < m < 2*n: m = sigma(k) for some k > 0, and 2*n - 1 - m
 *          and 2*n - 1 + m are both prime}|, where sigma(k) is the sum of all
 *          (positive) divisors of k.
 * https://oeis.org/A233864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < m < 2*n : m = sigma(k) for some k>0, and 2*n-1-m
 *            and 2*n-1+m are both prime }| (sigma(k) = sum of positive
 *            divisors of k)
 * Range:     u8 n <= 6737, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_trivial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_trivial.h"

/* -- simple primality test for 64 bit values -------------------------------- */

/* -------------------------------------------------------------------------- */
/* Static caches: sigma array and temporary bit array for seen m values.     */
/* The caches grow only when a larger n is requested.                        */
static unsigned long long *sigma_cache = NULL;   /* sigma values, index 1..2*n-1 */
static size_t sigma_cache_size = 0;              /* length of sigma_cache      */
static unsigned char *seen_cache = NULL;          /* bitmap of m values         */
static size_t seen_cache_size = 0;               /* size of seen_cache        */

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* undefined for n==0 */

    /* 1.  Ensure the sigma_cache is large enough for indices up to 2*n-1   */
    size_t required_sigma = (size_t)(2 * n);      /* we need indices 0..2*n-1 */
    if (required_sigma > sigma_cache_size) {
        /* Allocate new cache */
        unsigned long long *new_cache = (unsigned long long*)
            malloc(required_sigma * sizeof(unsigned long long));
        if (!new_cache) abort();                 /* out of memory */

        /* Compute sigma from scratch up to required_sigma-1 */
        for (size_t i = 0; i < required_sigma; ++i)
            new_cache[i] = 0;
        for (size_t i = 1; i < required_sigma; ++i) {
            for (size_t j = i; j < required_sigma; j += i) {
                new_cache[j] += i;
            }
        }

        /* Replace old cache */
        free(sigma_cache);
        sigma_cache = new_cache;
        sigma_cache_size = required_sigma;
    }

    /* 2.  Allocate or grow the seen_cache (size 2*n) */
    size_t required_seen = (size_t)(2 * n);
    if (required_seen > seen_cache_size) {
        unsigned char *new_seen = (unsigned char*)
            realloc(seen_cache, required_seen);
        if (!new_seen) abort();
        seen_cache = new_seen;
        seen_cache_size = required_seen;
    }
    /* Reset seen_cache */
    memset(seen_cache, 0, seen_cache_size);

    /* 3.  Count distinct m that satisfy the conditions */
    unsigned long long count = 0;
    const unsigned long long base = (unsigned long long)(2 * n - 1);

    for (idx_t k = 1; k < 2 * n; ++k) {
        unsigned long long m = sigma_cache[k];
        if (m == 0 || m >= 2 * n) continue;      /* not in 0 < m < 2*n */

        unsigned long long p1 = base - m;
        unsigned long long p2 = base + m;

        if (!is_prime_trivial(p1) || !is_prime_trivial(p2))
            continue;

        if (!seen_cache[(size_t)m]) {
            seen_cache[(size_t)m] = 1;
            ++count;
        }
    }

    return (seq_t)count;
}
