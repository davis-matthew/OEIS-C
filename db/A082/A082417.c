/*
 * A082417  Numbers k such that P(k) < P(k+1) > P(k+2), where P(k) is the
 *          largest prime factor of k (A006530).
 * https://oeis.org/A082417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = the n-th integer k such that P(k) < P(k+1) > P(k+2) */
/* Where P(m) is the largest prime factor of m (A006530). */

#include <stdlib.h>
#include <stdint.h>

/* Utility to compute the largest prime factor of an unsigned 64‑bit integer. */
static inline uint64_t _lpf_uint64(uint64_t n)
{
    if (n <= 1)
        return 1;
    uint64_t max = 1;

    /* Handle factor 2. */
    while ((n & 1ULL) == 0) {
        max = 2;
        n >>= 1;
    }

    /* Handle odd factors. */
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            max = i;
            n /= i;
        }
    }

    /* If remaining n is a prime > 2. */
    if (n > 1)
        max = n;

    return max;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Static state for memoisation.  All static objects are initialised
       only once and retain their values between calls. */
    static seq_t *cache     = NULL;   /* Array of found k values   */
    static size_t cache_cap = 0;      /* Allocated capacity        */
    static idx_t  found      = 0;      /* Number of terms stored   */
    static uint64_t k_next   = 2;      /* Next candidate k to test */

    /* Generate terms until we have at least n of them cached. */
    while (found < n) {
        /* Allocate or expand the cache when needed. */
        if (cache_cap <= (size_t)found) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* In a real build, handle allocation failure. */
            if (cache) {
                for (size_t i = 0; i < (size_t)found; ++i)
                    tmp[i] = cache[i];
                free(cache);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the next candidate k. */
        uint64_t pk   = _lpf_uint64(k_next);
        uint64_t pk1  = _lpf_uint64(k_next + 1);
        uint64_t pk2  = _lpf_uint64(k_next + 2);

        if (pk < pk1 && pk1 > pk2) {
            cache[found++] = (seq_t)k_next;
        }

        ++k_next;
    }

    return cache[n - 1];
}
