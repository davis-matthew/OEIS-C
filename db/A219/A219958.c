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
 * Validated: standard, pass, 79 terms through n = 115 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219958: find the n-th integer whose square can be written as
   2*p + 3*q with primes p, q (search‑based algorithm). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/sieve_primes.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        seq_t *new_arr = mem_resize(seq_t, seq_cache, new_cap);
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
            seq_t *new_arr = mem_resize(seq_t, seq_cache, new_cap);
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
