/*
 * A195270  3-gap primes: Prime p is a term iff there is no prime between 3*p
 *          and 3*q, where q is the next prime after p.
 * https://oeis.org/A195270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1038, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3-gap primes: p is a term iff there is no prime between 3*p and 3*q,
 * where q is the next prime after p. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* static initialization */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t prime_cap = 10000000;   /* big enough to cover ~10^4 terms */
    static idx_t prime_idx = 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (!initialized) {
        /* build list of primes up to prime_cap */
        build_primes_upto(prime_cap, &primes, &primes_cnt);
        cache = NULL; cache_cap = 0; cache_len = 0;
        prime_idx = 0;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Need at least two consecutive primes to test */
        if (prime_idx + 1 >= (idx_t)primes_cnt) break;

        uint64_t p = primes[prime_idx];
        uint64_t q = primes[prime_idx + 1];

        uint64_t start = 3ULL * p + 1ULL;
        uint64_t end   = 3ULL * q - 1ULL;
        bool ok = true;

        for (uint64_t x = start; x <= end; ++x) {
            if (is_prime((idx_t)x)) { ok = false; break; }
        }

        if (ok) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = (seq_t)p;
            ++cache_len;
        }
        ++prime_idx;
    }

    /* If somehow we ran out of primes (unlikely with chosen limit), simply
     * return 0. For the intended range (~10^4 terms) this will never happen. */
    if (n == 0 || cache_len < n) return 0;
    return cache[n - 1];
}
