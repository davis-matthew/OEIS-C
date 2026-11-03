/*
 * A178527  Primes p such that either p - 2 or p + 2 has more than two
 *          distinct prime divisors.
 * https://oeis.org/A178527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 4960, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9266 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p such that either p-2 or p+2 has more than two distinct
 * prime divisors.  Implements OEIS sequence A178527. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Requires helper from utils: */

/* Return true if x >= 2 is prime (deterministic trial division) */
static inline bool is_prime(idx_t x)
{
    if (x < 2)                         return false;
    if (x == 2 || x == 3)              return true;
    if (x % 2 == 0)                    return false;
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0)                return false;
    return true;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Return the n-th element of sequence A178527.  n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache for terms already found. */
    static seq_t *cache    = NULL;     /* array of found terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t  next_candidate = 2;  /* next number to test for primality */

    /* Expand cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next prime number. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Inspect this prime p. */
        uint64_t p = (uint64_t)next_candidate;
        int f1 = count_distinct_primes(p - 2);
        int f2 = count_distinct_primes(p + 2);

        if (f1 > 2 || f2 > 2) {
            /* Store the term in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) {
                    /* In a real program we would handle allocation failure.
                       For this problem we can assert or abort. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
        /* Move to the next candidate. */
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
