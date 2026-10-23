/*
 * A038873  Primes p such that 2 is a square mod p; or, primes congruent to
 *          {1, 2, 7} mod 8.
 * https://oeis.org/A038873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3246, u32 n <= 3247, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) – the n‑th prime p such that 2 is a quadratic residue mod p
 * (i.e., p == 2 or p ≡ 1 or 7 (mod 8)).  The sequence is indexed
 * starting at 1.  Values are computed lazily and cached; the function
 * works correctly for any order of calls.  The algorithm is a
 * straightforward prime test loop that skips numbers with the wrong
 * residue modulo 8. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static idx_t candidate = 3;          /* next candidate to test (after 2) */

#ifdef __cplusplus
    /* C++ version – use std::vector for proper construction/destruction */
    static std::vector<seq_t> cache;
#else
    /* C version – use malloc/realloc (seq_t is a POD in C builds) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* number of primes stored */
    static size_t cache_cap = 0;        /* allocated capacity (in seq_t units) */
#endif

    /* One‑time initialisation */
    if (!initialized) {
        initialized = true;
#ifdef __cplusplus
        cache.reserve(32);
        cache.push_back((seq_t)2);
#else
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache_len = 1;
#endif
        candidate = 3;
    }

    /* Return cached value if already known */
#ifdef __cplusplus
    if (n <= cache.size())
        return cache[n - 1];
#else
    if (n <= cache_len)
        return cache[n - 1];
#endif

    /* Generate new terms until we have at least n primes */
#ifdef __cplusplus
    while (cache.size() < n) {
#else
    while (cache_len < n) {
#endif
        /* Find next candidate that is ≡1 or 7 (mod 8) */
        while (true) {
            if (((candidate & 7ULL) == 1ULL) || ((candidate & 7ULL) == 7ULL)) {
                if (is_prime_big((seq_t)candidate)) {
                    /* Grow cache if needed */
#ifdef __cplusplus
                    cache.push_back((seq_t)candidate);
#else
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap * 2;
                        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)candidate;
#endif
                    ++candidate;
                    break;  /* a prime found – exit inner loop */
                }
            }
            ++candidate;
        }
    }

    /* Return the requested term */
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
