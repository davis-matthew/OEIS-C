/*
 * A211485  Numbers for which the canonical prime factorization contains only
 *          an odd number of exponents, all of which are congruent to 1 modulo
 *          3.
 * https://oeis.org/A211485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 915 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose canonical prime factorization contains only an
   odd number of exponents, all of which are congruent to 1 modulo 3. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

static seq_t *cache      = NULL;  /* stored terms */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t last_checked = 1; /* last integer examined */

/* Test whether n satisfies the condition. */
static bool is_good_uint64(uint64_t n) {
    if (n == 1) return true;          /* 1 is included by convention */

    uint64_t temp = n;
    size_t distinct = 0;

    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        if (p == 0) return false;     /* safety, should not occur */

        size_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        if (exp % 3U != 1U) return false; /* exponent not 1 mod 3 */
        ++distinct;
    }

    return (distinct & 1U) != 0;       /* odd number of distinct primes */
}

/* Grow the cache until it contains at least n terms. */
static void grow_cache_to(idx_t n) {
    if (cache_len >= (size_t)n) return;

    /* Allocate initial cache if necessary. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return;              /* allocation failure */
        cache[0] = (seq_t)1;              /* first term */
        cache_len = 1;
        last_checked = 1;
    }

    while (cache_len < (size_t)n) {
        ++last_checked;                   /* examine next integer */

        if (is_good_uint64(last_checked)) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but return 0 */
    grow_cache_to(n);
    return cache[n - 1];
}
