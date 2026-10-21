/*
 * A078576  Sum of distinct prime factors of the average of n-th twin prime
 *          pair.
 * https://oeis.org/A078576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008472(A014574(n))
 * Range:     u8 n <= 53, u16 n <= 3787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of distinct prime factors of the average of the n‑th twin prime pair */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* -------------------------------------------------------------------------- */
/*  Prime sieve state (rebuildable)                                           */
/* -------------------------------------------------------------------------- */
static uint64_t *g_primes   = NULL;   /* array of primes up to current limit */
static size_t   g_prime_cnt = 0;      /* number of primes stored           */
static uint64_t g_limit     = 4000000U;/* current sieve limit               */
static bool     g_initialized = false;

/* Rebuild the prime list up to a new upper bound. */
static void rebuild_primes_upto(uint64_t new_limit)
{
    if (g_primes != NULL) {
        free(g_primes);
        g_primes = NULL;
    }
    /* build_primes_upto will allocate memory for the array */
    build_primes_upto(new_limit, &g_primes, &g_prime_cnt);
    g_limit = new_limit;
    g_initialized = (g_primes != NULL);
}

/* Binary search on the prime array. */
static inline bool is_prime_in_list(uint64_t x)
{
    size_t lo = 0, hi = g_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = g_primes[mid];
        if (v == x) return true;
        if (v < x)  lo = mid + 1;
        else        hi = mid;
    }
    return false;
}

/* Find the base prime p of the n‑th twin prime pair (p, p+2). */
static uint64_t nth_twin_base(idx_t n)
{
    if (!g_initialized)
        rebuild_primes_upto(g_limit);

    while (true) {
        idx_t twin_cnt = 0;
        for (size_t i = 0; i < g_prime_cnt; ++i) {
            uint64_t p = g_primes[i];
            if (p + 2 > g_limit) break;            /* beyond current sieve */
            if (is_prime_in_list(p + 2)) {
                ++twin_cnt;
                if (twin_cnt == n) return p;        /* found the n‑th twin */
            }
        }
        /* Not enough twins – extend the sieve and try again. */
        uint64_t new_limit = g_limit * 2ULL;
        rebuild_primes_upto(new_limit);
    }
}

/* -------------------------------------------------------------------------- */
/*  OEIS sequence implementation                                              */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* offset is 1 */

    /* Locate the twin prime pair. */
    uint64_t p = nth_twin_base(n);             /* p in (p, p+2)            */
    uint64_t avg = p + 1;                      /* average of the pair      */

    /* Factor avg and sum distinct prime factors. */
    seq_t sum = 0;

    uint64_t prime_factors[64];
    uint64_t exponents[64];
    size_t factor_cnt;

    factor_u64(avg, prime_factors, exponents, &factor_cnt);

    for (size_t i = 0; i < factor_cnt; ++i) {
        sum += (seq_t)prime_factors[i];
    }

    return sum;
}
