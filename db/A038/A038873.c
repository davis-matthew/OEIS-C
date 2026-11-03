/*
 * A038873  Primes p such that 2 is a square mod p; or, primes congruent to
 *          {1, 2, 7} mod 8.
 * https://oeis.org/A038873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9797 (2026-11-03)
 * Helpers:   is_prime_u64
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
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
