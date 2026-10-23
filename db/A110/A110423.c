/*
 * A110423  a(n) = (n+1)(n+2)...(n+prime(k)) where prime(k) <= n < prime(k+1).
 * https://oeis.org/A110423
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{i=n+1}^{n+prime(k)} i where prime(k) ≤ n <
 *            prime(k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/is_prime.h"

static bool primes_initialized = false;
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static uint32_t primes_limit = 0;

/* Ensure we have a prime list covering at least up to max_n */
static void ensure_primes_up_to(uint64_t max_n) {
    uint32_t new_limit = (uint32_t)(max_n);
    /* add a small margin to avoid frequent re‑sieving */
    new_limit += 100;
    if (!primes_initialized || new_limit > primes_limit) {
        if (primes != NULL) free(primes);
        simple_sieve(new_limit, &primes, &prime_count);
        primes_limit = new_limit;
        primes_initialized = true;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* behaviour below the OEIS offset is irrelevant */

    /* Make sure the prime table covers n */
    ensure_primes_up_to((uint64_t)n);

    /* Find the largest prime <= n */
    uint32_t p;
    if (n <= primes[prime_count - 1]) {
        size_t lo = 0, hi = prime_count - 1, mid;
        p = 2;
        while (lo <= hi) {
            mid = (lo + hi) >> 1;
            uint32_t val = primes[mid];
            if (val <= n) {
                p = val;
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
    } else {
        /* Fallback for n larger than the current sieve limit */
        for (uint64_t x = n; x >= 2; --x) {
            if (is_prime(x)) { p = (uint32_t)x; break; }
        }
    }

    /* Compute the product (n+1)*(n+2)*...*(n+p) */
    seq_t result = 1;
    for (idx_t k = n + 1; k <= n + (idx_t)p; ++k) {
        result *= (seq_t)k;
    }
    return result;
}
