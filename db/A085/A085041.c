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
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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
        cache = mem_resize(seq_t, cache, new_len);
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
