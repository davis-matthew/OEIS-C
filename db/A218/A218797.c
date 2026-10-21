/*
 * A218797  Number of ways to write 2n - 1 as p + q + r with p <= q <= r and
 *          p, q, r, p^2 + q^2 + r^2 all prime.
 * https://oeis.org/A218797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, isPrimeSmall
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A218797: a(n) = number of triples (p,q,r) with
 *  p <= q <= r, p+q+r = 2n-1, p, q, r, and p²+q²+r² prime.  */
/*  The function uses a brute‑force search over primes up to 2n‑1.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeSmall.h"

/* ------------------------------------------------------------------ */
/*  Global memoisation and prime tables                               */
/* ------------------------------------------------------------------ */

static uint64_t *g_primes = NULL;      /* array of primes up to g_prime_limit */
static size_t g_primes_cnt = 0;

static bool *g_prime_bool = NULL;      /* 1 if index is prime, size g_prime_limit+1 */
static idx_t g_prime_limit = 0;        /* current upper limit of prime tables */

static seq_t *g_cache = NULL;          /* cache of computed values */
static bool  *g_compiled = NULL;       /* flags if g_cache[i] is valid */
static size_t g_cache_cap = 0;         /* current capacity of the above arrays */

/* ------------------------------------------------------------------ */
/*  Ensure that we have primes up to 'limit'.                          */
/* ------------------------------------------------------------------ */
static void
ensure_primes(idx_t limit)
{
    if (limit <= g_prime_limit)
        return;

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    free(g_primes);
    free(g_prime_bool);

    g_primes = new_primes;
    g_primes_cnt = new_cnt;

    g_prime_bool = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    for (size_t i = 0; i < g_primes_cnt; ++i)
        g_prime_bool[(size_t)g_primes[i]] = true;

    g_prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the cache arrays are large enough for index 'm'.       */
/* ------------------------------------------------------------------ */
static void
ensure_cache_capacity(idx_t m)
{
    if (m < g_cache_cap)
        return;

    size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 128;
    while ((idx_t)(new_cap) <= m)
        new_cap <<= 1;

    g_cache     = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    g_compiled  = (bool  *)realloc(g_compiled, new_cap * sizeof(bool));

    /* Mark newly allocated slots as not computed. */
    for (size_t i = g_cache_cap; i < new_cap; ++i)
        g_compiled[i] = false;

    g_cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence routine                                               */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)            /* outside of defined offset, return 0 */
        return 0;

    /* Grow cache if necessary.  This is safe for repeated calls in any order. */
    ensure_cache_capacity(n);
    if (g_compiled[(size_t)n])
        return g_cache[(size_t)n];

    /* Target sum to represent.  For n >= 1, 2n-1 is odd and > 0. */
    idx_t target = 2 * n - 1;

    /* Build primes up to 'target' (needed for p,q,r). */
    ensure_primes((idx_t)target);

    seq_t count = 0;

    /* Brute‑force over triples of primes p <= q <= r with p+q+r = target. */
    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (p > target)
            break;
        for (size_t j = i; j < g_primes_cnt; ++j) {
            uint64_t q = g_primes[j];
            uint64_t pq = p + q;
            if (pq > target)
                break;
            uint64_t r = target - pq;
            if (r < q)                   /* enforce p <= q <= r */
                break;                  /* r decreases as j increases */
            if (r > g_prime_limit)       /* not in current table */
                continue;
            if (!g_prime_bool[(size_t)r])
                continue;               /* r is not prime */

            /* Sum of squares fits in 32‑bit signed before casting. */
            uint64_t ss = (uint64_t)p * p + (uint64_t)q * q + (uint64_t)r * r;
            if (isPrimeSmall((int)ss))
                ++count;
        }
    }

    g_cache[(size_t)n]   = count;
    g_compiled[(size_t)n] = true;
    return count;
}
