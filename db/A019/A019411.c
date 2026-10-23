/*
 * A019411  Primes with primitive root 89.
 * https://oeis.org/A019411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, is_prime, mod_pow, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 89 is a primitive root modulo p */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"         /* seq_t mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/seq_alloc.h"       /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"     /* seq_t *seq_realloc(seq_t*, size_t, size_t) */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t) */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t) */

/* Cached list of terms.  cache[i] holds a(i+1). */
static seq_t *cache      = NULL;  /* memoised values */
static size_t cache_size = 0;     /* number of cached terms */
static size_t cache_cap  = 0;     /* allocated storage capacity */

static uint64_t last_prime = 1;   /* last prime produced by get_next_prime */

/* Return the next prime >= 2 greater than last_prime, updating last_prime. */
static uint64_t get_next_prime(void)
{
    uint64_t cand;
    if (last_prime < 2)
        cand = 2;
    else
        cand = last_prime + 1;

    for (;;) {
        if (cand == 2) {
            last_prime = 2;
            return 2;
        }
        if ((cand & 1ULL) == 0) {          /* even number >2, skip */
            cand++;
            continue;
        }
        if (is_prime(cand)) {
            last_prime = cand;
            return cand;
        }
        cand += 2;                         /* skip even candidates */
    }
}

/* Test whether 89 is a primitive root modulo p. */
static bool is_primitive_root_89(uint64_t p)
{
    /* 89 is coprime to p except when p == 89 itself. */
    if (p == 2)
        return true;                      /* 1 generates the group modulo 2 */
    if (p == 89)
        return false;                     /* base is 0 modulo p, cannot be a generator */

    uint64_t m = p - 1;
    uint64_t uniq[64];
    size_t uniq_cnt = 0;

    /* Collect distinct prime factors of p-1. */
    while (m > 1) {
        uint64_t f = _smallest_prime_factor(m);
        /* keep each factor only once */
        bool already = false;
        for (size_t i = 0; i < uniq_cnt; ++i) {
            if (uniq[i] == f) {
                already = true;
                break;
            }
        }
        if (!already)
            uniq[uniq_cnt++] = f;
        while (m % f == 0)
            m /= f;
    }

    /* Check that 89^( (p-1)/q ) != 1 for each distinct factor q. */
    for (size_t i = 0; i < uniq_cnt; ++i) {
        uint64_t exp = (p - 1) / uniq[i];
        seq_t r = mod_pow((seq_t)89, exp, (seq_t)p);
        if (r == (seq_t)1)
            return false;                 /* order is proper divisor */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* out of range, harmless */

    if (cache_size >= (size_t)n)
        return cache[n - 1];

    /* Ensure cache storage exists. */
    if (cache_cap == 0) {
        cache_cap  = 16;
        cache      = seq_alloc(cache_cap);
    }

    while (cache_size < (size_t)n) {
        uint64_t p = get_next_prime();

        /* grow cache if necessary */
        if (cache_cap <= cache_size) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        if (is_primitive_root_89(p))
            cache[cache_size++] = (seq_t)p;
    }

    return cache[n - 1];
}
