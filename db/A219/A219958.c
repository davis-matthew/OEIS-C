/*
 * A219958  Integers n such that n^2 can be represented as 2p + 3q with p and
 *          q primes not necessarily distinct.
 * https://oeis.org/A219958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 107, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   is_prime_big, next_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219958: find the n-th integer whose square can be written as
   2*p + 3*q with primes p, q (search‑based algorithm). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/sieve_primes.h"
#include "../utils/next_prime.h"

/* --------------------------------------------------------------------- */
/* Small prime table (up to 1,000,000) used for quick searches           */
/* --------------------------------------------------------------------- */
static idx_t *small_primes     = NULL;            /* array of primes  */
static size_t small_prime_cnt  = 0;               /* number of primes */
static bool   small_primes_init = false;         /* init flag        */

/* initialise the small prime table once */
static void init_small_primes(void)
{
    if (!small_primes_init) {
        /* generate all primes <= 1,000,000 */
        sieve_primes(1000000, &small_primes, &small_prime_cnt);
        small_primes_init = true;
    }
}

/* --------------------------------------------------------------------- */
/* Checks whether the integer x belongs to the sequence                */
/* --------------------------------------------------------------------- */
static bool in_sequence(const idx_t n)
{
    init_small_primes();

    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 as seq_t */
    if (sq < 6)                      /* smallest representable value is 6 = 2*3 + 3*? */
        return false;

    seq_t limitP = sq / 2;            /* p must not exceed n^2 / 2 */

    /* 1. Try all small primes first */
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        seq_t p = (seq_t)small_primes[i];
        if (p > limitP)
            break;

        seq_t diff = sq - 2 * p;
        if (diff % 3 != 0)
            continue;

        seq_t q = diff / 3;
        if (q >= 2 && is_prime_big(q))
            return true;
    }

    /* 2. Continue with bigger primes generated on demand */
    if (!small_primes)  /* should never happen, but guard anyway */
        return false;

    idx_t start = small_primes[small_prime_cnt - 1] + 1;
    while (true) {
        idx_t cand = next_prime(start);      /* next prime >= start */
        seq_t p = (seq_t)cand;
        if (p > limitP)
            break;                           /* beyond the allowed range */

        seq_t diff = sq - 2 * p;
        if (diff % 3 == 0) {
            seq_t q = diff / 3;
            if (q >= 2 && is_prime_big(q))
                return true;
        }
        start = cand + 1;
    }

    return false;   /* no representation found */
}

/* --------------------------------------------------------------------- */
/* Cache for already computed terms                                        */
/* --------------------------------------------------------------------- */
static seq_t *seq_cache        = NULL;   /* dynamic array of cached terms */
static size_t cache_len        = 0;       /* number of cached terms */
static size_t cache_cap        = 0;       /* current capacity of seq_cache */
static idx_t  next_candidate   = 1;       /* next n to test for inclusion */

/* ensure the cache has at least (n) terms; compute missing terms */
static void ensure_cache_size(idx_t n)
{
    if (cache_len >= n)
        return;

    /* grow cache if necessary */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 256 : cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_arr)
            abort();                     /* allocation failure */
        seq_cache = new_arr;
        cache_cap = new_cap;
    }

    while (cache_len < n) {
        /* find next n that satisfies the sequence property */
        while (!in_sequence(next_candidate))
            ++next_candidate;

        /* store it */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!new_arr)
                abort();
            seq_cache = new_arr;
            cache_cap = new_cap;
        }
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;                 /* prepare for the next search */
    }
}

/* --------------------------------------------------------------------- */
/* required external function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();                         /* offset starts at 1 */
    ensure_cache_size(n);
    return seq_cache[n - 1];
}
