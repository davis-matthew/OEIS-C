/*
 * A057605  Nonnegative n such that 4*n^2 + 163 is prime.
 * https://oeis.org/A057605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mod_pow_uint64, mul_mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th non‑negative integer m such that
 * 4*m*m + 163 is prime.  a(1) = 0, a(2) = 1, a(3) = 2, … */

/* This implementation keeps a growing cache of the found values and
 * uses a deterministic Miller‑Rabin test for 64‑bit integers (which
 * suffices for all candidates up to at least several thousand).
 *
 * The function works for any seq_t/idx_t definitions supplied by the
 * build environment, and does not depend on any external helper
 * functions. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow_uint64.h"
#include "../utils/mul_mod_uint64.h"

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */

/* deterministic Miller–Rabin for values < 2^64.
 * Bases {2, 3, 5, 7, 11, 13, 17} are sufficient for all 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const unsigned small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* ---------- sequence generation ---------- */

seq_t a(idx_t n)
{
    /* static cache of the first terms found */
    static seq_t *cache  = NULL;
    static idx_t cache_len = 0;   /* number of stored terms      */
    static idx_t cache_cap = 0;   /* capacity of the array       */
    static uint64_t next_n = 0;   /* next integer to test       */

    /* initialise the cache on first call */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) /* allocation failure – return zero (unlikely test) */
            return (seq_t)0;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t cand = 4ULL * next_n * next_n + 163ULL;
        if (is_prime_uint64(cand)) {
            /* grow array if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – return zero */
                    return (seq_t)0;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_n;
        }
        ++next_n;
    }

    return cache[n - 1];
}
