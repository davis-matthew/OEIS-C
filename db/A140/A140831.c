/*
 * A140831  Numbers in whose canonical prime factorization the powers of the
 *          primes do not form an increasing sequence.
 * https://oeis.org/A140831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A140831: a(n) is the n‑th positive integer whose
 * list of prime powers p^b(n,p) (p prime) is not sorted in the same
 * order as the list of primes themselves.
 * The first terms are: 12, 24, 40, 45, 48, … (offset = 1).  */

/* acmph formula is not a simple closed form; this implementation
 * counts numbers by direct generation.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t *cache = NULL;          /* cached sequence values        */
static size_t cache_cap = 0;         /* cache capacity                */
static size_t cache_len = 0;         /* how many terms have been cached */

static uint64_t *primes = NULL;      /* list of primes up to prime_limit */
static size_t prime_cnt = 0;         /* number of primes in the list    */
static uint64_t prime_limit = 0;     /* largest prime stored           */

/* Ensure that primes[0..prime_cnt-1] contain all primes <= limit. */
static void
ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;
    /* Rebuild the prime table from scratch (the size is relatively
     * small for the range we need up to ~10^6, so this is inexpensive). */
    size_t cnt;
    uint64_t *newp;
    build_primes(limit, &newp, &cnt);

    free(primes);
    primes = newp;
    prime_cnt = cnt;
    prime_limit = limit;
}

/* Test whether a non‑zero candidate is in the sequence. */
static bool
in_sequence(uint64_t cand)
{
    /* Gather the prime factorization.  Distinct primes are kept in
     * ascending order because we traverse the primes array in order. */
    uint64_t pm[32];
    uint8_t  e[32];
    size_t   cnt = 0;
    uint64_t tmp = cand;

    /* Factor by primes up to sqrt(tmp). */
    uint64_t sqrt_tmp = _floor_isqrt_u64(tmp);
    ensure_primes_up_to(sqrt_tmp);

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint8_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            pm[cnt] = p;
            e[cnt]   = exp;
            ++cnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor > sqrt(cand) */
        pm[cnt] = tmp;
        e[cnt]   = 1;
        ++cnt;
    }

    /* Compare the values p^e in prime order.  If any adjacent pair
     * violates increasing order, the integer is in the sequence. */
    uint64_t prev_val = 0;
    bool first = true;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t val = 1;
        for (uint8_t k = 0; k < e[i]; ++k) val *= pm[i]; /* p^e   ≤ cand */

        if (!first) {
            if (val < prev_val) return true;          /* order mismatch */
        }
        prev_val = val;
        first = false;
    }
    return false;   /* monotone, not in sequence */
}

/* Cache a new term.  The cache is enlarged in doubling steps. */
static void
cache_term(uint64_t cand)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) { free(cache); abort(); }
        cache = p;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1, but harmless */

    /* Generate terms until we have at least n of them. */
    uint64_t next_candidate = 2;          /* start from 2 (the first positive >1) */
    if (cache_len > 0) next_candidate = (uint64_t)cache[cache_len - 1] + 1;

    while (cache_len < (size_t)n) {
        if (in_sequence(next_candidate)) cache_term(next_candidate);
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
