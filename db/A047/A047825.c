/*
 * A047825  Becomes prime after exactly 6 iterations of f(x) = sum of prime
 *          factors of x.
 * https://oeis.org/A047825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2306, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   build_spf, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A047825: numbers that become prime after exactly six
    iterations of f(x) = sum of distinct prime factors (without
    multiplicity). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------ */
/*  Global data                                               */
/* ------------------------------------------------------------ */
static idx_t   *spf      = NULL;   /* smallest prime factor array   */
static uint64_t spf_limit = 0;      /* current size of spf           */

static seq_t *cache     = NULL;   /* cached sequence values        */
static size_t cache_len = 0;      /* number of cached entries      */
static size_t cache_cap = 0;      /* current capacity              */

static uint64_t last_checked = 0; /* last integer examined         */

/* ------------------------------------------------------------ */
/*  Ensure that the SPF array covers at least `needed`.        */
/* ------------------------------------------------------------ */
static void ensure_spf_limit(uint64_t needed)
{
    if (needed <= spf_limit)
        return;

    uint64_t new_limit = (spf_limit) ? spf_limit << 1 : 1000;
    while (new_limit < needed)
        new_limit <<= 1;

    idx_t *new_spf = NULL;
    build_spf((idx_t)new_limit, &new_spf);

    free(spf);
    spf      = new_spf;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------ */
/*  Sum of distinct prime factors of `x`.  Uses the SPF table   */
/*  when possible, otherwise simple trial division.            */
/* ------------------------------------------------------------ */
static uint64_t sum_distinct_prime_factors(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 1) {
        uint64_t p;
        if (x <= spf_limit) {          /* use SPF table */
            p = (uint64_t)spf[x];
        } else {                         /* fallback */
            p = 0;
            for (uint64_t d = 2; d * d <= x; ++d) {
                if (x % d == 0) { p = d; break; }
            }
            if (!p) p = x;               /* x itself is prime */
        }
        sum += p;
        while (x % p == 0) x /= p;
    }
    return sum;
}

/* ------------------------------------------------------------ */
/*  Test whether `val` becomes prime after exactly six          */
/*  iterations of f().                                         */
/* ------------------------------------------------------------ */
static bool satisfies(uint64_t val)
{
    uint64_t cur = val;
    for (int step = 1; step <= 6; ++step) {
        cur = sum_distinct_prime_factors(cur);
        if (step < 6) {
            if (is_prime((idx_t)cur))
                return false;           /* became prime too early */
        } else {
            return is_prime((idx_t)cur); /* prime after 6 steps */
        }
    }
    return false;   /* should never reach */
}

/* ------------------------------------------------------------ */
/*  Append a new element to the cache, growing it as needed.    */
/* ------------------------------------------------------------ */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap) ? (cache_cap << 1) : 16;
        seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------ */
/*  Public function: return a(n) for any n >= 1.                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for offset 0 */

    /* Generate terms until we have at least n values cached. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        ensure_spf_limit(last_checked);
        if (satisfies(last_checked))
            cache_push((seq_t)last_checked);
    }

    return cache[n - 1];
}
