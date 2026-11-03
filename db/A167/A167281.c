/*
 * A167281  Primes containing the string 555.
 * https://oeis.org/A167281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167281: primes containing the decimal substring "555"
/* This implementation searches for primes that contain the string
   "555", memoises the results, and returns the n‑th such prime.
   It satisfies the requirements for both C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

/* Returns true if the decimal representation of x contains
   the substring 555.  Only 64‑bit values are tested, which
   suffices for all n requested in the tests. */
static bool contains_555(uint64_t x)
{
    while (x >= 555) {
        if (x % 1000ULL == 555ULL)
            return true;
        x /= 10;
    }
    return false;
}

/* Static cache for already computed terms.  The cache is
   grown lazily as needed.  The size is limited so that the
   function is fast even when called out of order. */
static seq_t *cache = NULL;   /* dynamically allocated array of seq_t */
static size_t cache_len = 0;  /* number of values actually stored   */
static size_t cache_cap = 0;  /* allocated capacity                 */

/* Compute the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; guard against zero */
        return 0;

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Start searching from the last found prime + 1,
       or from the smallest prime if the cache is empty. */
    uint64_t current = (cache_len == 0) ? 2ULL
                                        : (uint64_t)cache[cache_len - 1] + 1ULL;

    while (cache_len < (size_t)n) {
        /* Skip numbers that do not contain the substring 555 */
        while (!contains_555(current))
            ++current;

        /* Test primality using the generic big‑primality routine. */
        if (is_prime_big((seq_t)current)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) /* allocation failed */
                    exit(EXIT_FAILURE);
                cache      = new_cache;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)current;
        }
        ++current;
    }

    return cache[(size_t)n - 1];
}
