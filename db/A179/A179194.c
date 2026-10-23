/*
 * A179194  Bases n in which 1/(n-2) is non-terminating and has period n-3.
 * https://oeis.org/A179194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, powmod_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179194: Bases n in which 1/(n-2) has maximal period (n-3).
   The nth term is computed by sieving primes and testing whether 2
   is a primitive root modulo p; the term is then p + 2. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/build_spf.h"          /* build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/powmod_two.h"

static uint64_t *primes = NULL;          /* array of primes up to current_limit */
static size_t    primes_len = 0;        /* number of primes found */
static size_t    primes_processed = 0;  /* next prime index to test */

static idx_t  *spf = NULL;              /* smallest prime factor array */
static size_t spf_cap = 0;               /* spf array size */

static seq_t *cache = NULL;             /* cached sequence values */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Test whether 2 is a primitive root modulo prime p (p > 2). */
static bool is_primitive_root_2(uint64_t p)
{
    uint64_t t = p - 1;
    uint64_t temp = t;
    uint64_t last_q = 0;
    while (temp > 1) {
        uint64_t q = spf[temp];
        if (q != last_q) {
            uint64_t exp = t / q;
            if (powmod_two(exp, p) == 1ULL)
                return false;
            last_q = q;
        }
        temp /= q;
    }
    return true;
}

/* Ensure that the cache contains at least n values. */
static void ensure_cache(idx_t n)
{
    static size_t current_limit = 0;

    if (current_limit == 0) {
        current_limit = 1000000ULL;                 /* initial sieve limit */
        build_primes_upto((idx_t)current_limit, &primes, &primes_len);
        build_spf((idx_t)current_limit, &spf);
        spf_cap = current_limit + 1;
    }

    while (cache_len < n) {
        /* Process remaining primes in the current list */
        while (primes_processed < primes_len) {
            uint64_t p = primes[primes_processed++];
            if (p <= 2ULL) continue;              /* skip 2 */
            if (is_primitive_root_2(p)) {
                /* add p + 2 to cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_ptr) exit(EXIT_FAILURE);
                    cache = new_ptr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)(p + 2ULL);
                if (cache_len >= n) break;
            }
        }

        /* If still not enough, extend the sieve */
        if (cache_len < n) {
            current_limit *= 2;
            free(primes);
            primes = NULL;
            build_primes_upto((idx_t)current_limit, &primes, &primes_len);

            free(spf);
            spf = NULL;
            build_spf((idx_t)current_limit, &spf);
            spf_cap = current_limit + 1;

            primes_processed = 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (cache_len < n) ensure_cache(n);
    return cache[n - 1];
}
