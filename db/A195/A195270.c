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
 * Validated: standard, pass, 62 terms through n = 9630 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3-gap primes: p is a term iff there is no prime between 3*p and 3*q,
 * where q is the next prime after p. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
