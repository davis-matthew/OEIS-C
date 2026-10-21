/*
 * A240710  Primes p such that at least one number among p+-6 and p+-12 is
 *          also a prime.
 * https://oeis.org/A240710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 4804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240710: primes p such that at least one of p±6 or p±12 is prime
   (computed by filtering primes with neighboring primality checks). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t   *term_cache  = NULL; /* cache of computed terms */
static size_t  cache_cap   = 0;     /* capacity of the cache array   */
static size_t  cache_len   = 0;     /* number of terms stored        */
static uint64_t next_candidate = 5; /* next odd number to test       */
static bool     initialized    = false;

/* Ensure the cache has capacity for at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cache = tmp;
    cache_cap  = new_cap;
}

/* Return the n-th term of A240710 (1‑based index). */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        next_candidate = 5;
    }

    while (cache_len < n) {
        uint64_t p = next_candidate;
        if (is_prime(p)) {
            bool good = false;
            const int offs[2] = { 6, 12 };
            for (int i = 0; i < 2 && !good; ++i) {
                int o = offs[i];
                if (p > (uint64_t)o && is_prime((idx_t)(p - o)))
                    good = true;
                if (is_prime((idx_t)(p + o)))
                    good = true;
            }
            if (good) {
                ensure_cache(cache_len + 1);
                term_cache[cache_len++] = (seq_t)p;
            }
        }
        next_candidate += 2; /* skip even numbers */
    }

    return term_cache[n - 1];
}
