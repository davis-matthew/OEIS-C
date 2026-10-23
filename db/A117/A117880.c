/*
 * A117880  a(1) = 4; a(n) is smallest semiprime > 2*a(n-1).
 * https://oeis.org/A117880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A117880 – Smallest semiprime > 2*a(n‑1)
 * a(1) = 4
 * a(n) = min{ s ∈ S : s > 2*a(n‑1) }   where S = {..., pq | p,q primes}
 *
 * The implementation stores computed terms in a static cache and
 * grows it as needed.  To test whether a candidate is a semiprime
 * we try to divide it by a precomputed list of primes up to 1 000 000.
 * The first divisor found is used to check whether the co‑factor is
 * also prime (using the library routine is_prime_big).  This is
 * sufficient for all terms that fit within the tested range.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/*  Helper: maintain a static list of primes up to a dynamic limit   */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;
static size_t  primes_cnt = 0;
static uint64_t primes_limit = 0;
#define INIT_PRIME_LIMIT 1000000

static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_limit >= limit)
        return;
    /* free old list and build a new one */
    if (primes != NULL)
        free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether x is a semiprime (product of exactly two primes)     */
/* ------------------------------------------------------------------ */
static bool is_semiprime(seq_t x)
{
    if (x < (seq_t)4)          /* the smallest semiprime is 4 */
        return false;

    seq_t sqrtx = floor_sqrt(x);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t   ps = (seq_t)p;
        if (ps > sqrtx)
            break;
        if ((x % ps) == (seq_t)0) {
            seq_t q = x / ps;
            if (q == ps)            /* perfect square of a prime */
                return true;
            return is_prime_big(q);  /* check if co‑factor is prime */
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache: index 1 maps to a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_up_to = 0;

    if (n == 0)
        return (seq_t)0;      /* undefined, but safe */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;  /* start with some room */
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)4;      /* a(1) = 4 */
        cache_up_to = 1;
        ensure_primes_up_to(INIT_PRIME_LIMIT);
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev * (seq_t)2 + (seq_t)1;   /* start just above 2*prev */

        while (!is_semiprime(cand)) {
            cand += (seq_t)1;
        }
        cache[i] = cand;
    }
    cache_up_to = (size_t)n;

    return cache[n];
}
