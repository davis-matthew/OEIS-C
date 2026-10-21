/*
 * A083574  Triangle read by rows: the prime factors of composite odd n with
 *          multiplicity.
 * https://oeis.org/A083574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A083574: the prime factors (with multiplicity) of odd composite numbers,
   read row‑by‑row.
   The function generates odd composites in increasing order, factors each
   into its prime factors, stores all factors in a static cache, and returns
   the nth factor (1‑based).  A simple trial division is used to find the
   smallest prime factor, avoiding any missing or incorrectly named utilities. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

static uint64_t _smallest_prime_factor(uint64_t n)
{
    /* Assumes n is an odd composite > 1 */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) return p;
    }
    return n;  /* n is prime (should not happen for composites) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of OEIS range – ignore */

    /* static cache of already generated prime factors */
    static seq_t *cache = NULL;      /* array of factors */
    static size_t cache_len = 0;     /* number of factors stored */
    static size_t cache_cap = 0;     /* current capacity of cache */

    /* state for generating subsequent odd composites */
    static uint64_t next_odd = 9;    /* next odd number to examine */
    static idx_t generated = 0;      /* total factors generated so far */

    /* return cached value if it already exists */
    if (n <= (idx_t)cache_len) {
        return cache[n - 1];
    }

    /* ensure enough capacity for at least one more element */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* generate factors until we have at least n of them */
    while (generated < n) {
        /* skip odd primes */
        if (is_prime(next_odd)) {
            next_odd += 2;
            continue;
        }

        /* factor the odd composite */
        uint64_t m = next_odd;
        while (m > 1) {
            uint64_t p = _smallest_prime_factor(m);
            /* ensure room for the new factor */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
            ++generated;
            m /= p;
        }
        next_odd += 2;  /* go to the next odd number */
    }

    return cache[n - 1];
}
