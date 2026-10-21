/*
 * A090093  a(n) is the smallest composite number coprime to n, n+1, n+2, n+3.
 * https://oeis.org/A090093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053672(n)^2 (A053672(n) = smallest prime coprime to n,
 *            n+1, n+2, n+3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Simple gcd for uint64_t */

/* Prime sieve – up to a limit that comfortably covers all needed primes */
static void build_prime_sieve(size_t limit, uint32_t **primes_out, size_t *cnt_out) {
    bool *is_composite = (bool*)malloc(limit + 1);
    if (!is_composite) return;
    for (size_t i = 0; i <= limit; ++i) is_composite[i] = false;
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            ++count;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    *primes_out = (uint32_t*)malloc(count * sizeof(uint32_t));
    if (!(*primes_out)) {
        free(is_composite);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) (*primes_out)[idx++] = (uint32_t)i;
    }
    *cnt_out = count;
    free(is_composite);
}

/* Global prime cache */
static uint32_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static const size_t PRIME_CACHE_LIMIT = 200000;

/* Ensure primes are precomputed */
static void ensure_primes(void) {
    if (prime_cache) return;
    build_prime_sieve(PRIME_CACHE_LIMIT, &prime_cache, &prime_cache_cnt);
}

/* Computes the smallest prime coprime to n, n+1, n+2, n+3 */
static uint64_t smallest_coprime_prime(idx_t n) {
    ensure_primes();
    idx_t seq_vals[4] = { n, n + 1, n + 2, n + 3 };
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint32_t p = prime_cache[i];
        bool ok = true;
        for (int j = 0; j < 4; ++j) {
            if (gcd_u64(p, seq_vals[j]) != 1) {
                ok = false;
                break;
            }
        }
        if (ok) return (uint64_t)p;
    }
    /* Fallback: extend sieve if needed – unlikely for n <= 10000 */
    size_t new_limit = PRIME_CACHE_LIMIT * 2;
    uint32_t *new_primes = NULL;
    size_t new_cnt = 0;
    do {
        build_prime_sieve(new_limit, &new_primes, &new_cnt);
        uint64_t result = 0;
        for (size_t i = 0; i < new_cnt; ++i) {
            uint32_t p = new_primes[i];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                if (gcd_u64(p, seq_vals[j]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) { result = p; break; }
        }
        if (result) {
            free(prime_cache);
            prime_cache = new_primes;
            prime_cache_cnt = new_cnt;
            return result;
        }
        free(new_primes);
        new_limit *= 2;
    } while (1);
    return 0; /* Should never reach here */
}

seq_t a(idx_t n) {
    uint64_t p = smallest_coprime_prime(n);
    return (seq_t)p * p;
}
