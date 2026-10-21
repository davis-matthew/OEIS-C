/*
 * A086097  Numbers n such that n and n+1 are semiprimes with a semiprime
 *          number of 1's in their binary representation.
 * https://oeis.org/A086097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 617, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_u64, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086097: Numbers n such that n and n+1 are semiprimes
   and the number of 1's in the binary representation of n and n+1
   are each semiprime.  This program searches numbers sequentially
   and caches the found terms.  It works for any seq_t / idx_t
   combination supported by the framework. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/is_prime_u64.h"

/* Pre‑computed table of whether 0…64 are semiprime (product of two primes,
   allowing a square).  This table is used for the popcount test. */
static bool semiprime_small[65];
static bool semiprime_small_inited = false;

static bool is_prime_small(int n) {
    if (n < 2) return false;
    for (int d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

static bool init_semiprime_small(void) {
    for (int n = 0; n <= 64; ++n) {
        if (n < 4) { semiprime_small[n] = false; continue; }
        bool ok = false;
        for (int i = 2; i <= n / i; ++i) {
            if (n % i == 0) {
                int j = n / i;
                if (is_prime_small(i) && is_prime_small(j)) {
                    ok = true;
                    break;
                }
            }
        }
        semiprime_small[n] = ok;
    }
    semiprime_small_inited = true;
    return true;
}

/* Popcount (number of 1 bits) for a 64‑bit integer */
static int popcount64(uint64_t v) {
    int c = 0;
    while (v) { v &= v - 1; ++c; }
    return c;
}

/* Test whether the number of 1's in the binary representation of v is semiprime */
static bool popcnt_is_semiprime(uint64_t v) {
    if (!semiprime_small_inited) init_semiprime_small();
    int pc = popcount64(v);
    return semiprime_small[pc];
}

/* Semiprime test for arbitrary 64‑bit numbers.
   Uses the smallest_prime_factor helper and a fast primality test. */
static bool is_semiprime_u64(uint64_t n) {
    if (n < 4) return false;          /* 0,1,2,3 are not semiprime */
    uint64_t p = smallest_prime_factor(n);
    if (p == 0 || p == n) return false;  /* prime or 1 */
    uint64_t q = n / p;
    if (q == 1) return false;            /* only one factor */
    return is_prime_u64(q);
}

/* Predicate for a candidate number */
static bool candidate_ok(uint64_t x) {
    if (!is_semiprime_u64(x))      return false;
    if (!is_semiprime_u64(x + 1))  return false;
    if (!popcnt_is_semiprime(x))   return false;
    if (!popcnt_is_semiprime(x + 1)) return false;
    return true;
}

/* Cache for already found terms – grows as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;     /* first integer to test */

/* Ensure the cache contains at least n_needed terms (1‑based index). */
static void ensure_cache(idx_t n_needed) {
    if (cache_len >= (size_t)n_needed) return;
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < (size_t)n_needed) {
        /* Skip non‑candidates */
        while (!candidate_ok(next_candidate)) ++next_candidate;
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;   /* next search starts at the following integer */
    }
}

/* Public function – returns the n‑th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n=0, guard */
    ensure_cache(n);
    return cache[n - 1];
}
