/*
 * A085041  a(1) = 2, a(n+1) = smallest prime of the form a(n) + k*prime(n+1),
 *          k >1.
 * https://oeis.org/A085041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(n+1) = smallest prime of the form a(n) +
 *            k*prime(n+1), k > 1
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;

    /* Static array of the first 200 000 integers' primes (enough for n≈10000) */
    static uint64_t *primes       = NULL;
    static size_t   primes_count = 0;
    static bool     primes_ready = false;

    /* Ensure the prime table is generated once */
    if (!primes_ready) {
        const idx_t prime_limit = 200000;          /* upper bound for prime(n) when n≈10000 */
        build_primes_upto(prime_limit, &primes, &primes_count);
        primes_ready = true;
    }

    /* Grow cache if needed */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* Base case */
    if (n == 1) {
        cache[1] = 2;
        return 2;
    }

    /* If already computed, return cached value */
    if (cache[n] != 0)
        return cache[n];

    /* Compute consecutive terms up to n, starting from the first missing position */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0)
            continue;

        seq_t prev = cache[i - 1];
        seq_t step = (seq_t)primes[i - 1];        /* prime(i) = primes[i-1] */

        for (idx_t k = 2;; ++k) {
            seq_t cand = prev + step * (seq_t)k;
            if (isPrimeGeneric(cand)) {
                cache[i] = cand;
                break;
            }
        }
    }

    return cache[n];
}
