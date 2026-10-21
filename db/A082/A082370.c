/*
 * A082370  a(n) = number of sets of consecutive primes whose arithmetic mean
 *          is A000040(n).
 * https://oeis.org/A082370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4000, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/********************************************************************
 * Sequence: A082370
 *
 * a(n) = number of sets of consecutive primes whose arithmetic mean
 * equals the n-th prime (A000040(n)).
 *
 * Algorithm:
 *   For the target prime P = prime[n‑1] let
 *        V_k = Σ_{i=0}^{k-1} prime[i] – P * k   (k = 0…π)
 *   where π is the number of pre‑computed primes.
 *   Any pair k1 < k2 with V_k1 = V_k2 corresponds to an interval
 *   of consecutive primes whose mean is P.  Thus a(n) equals the
 *   number of equal pairs among the V_k values.
 *   The values are computed in 64‑bit integers, sorted, then
 *   equal runs counted.  Results are cached per n.
 *
 * Complexity: O(π log π) per distinct n, with π ≈ 2 × 10⁴ for n ≤ 10 000.
 ********************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -----------------------------------------------------------------
 * Prime table – sufficient for n ≤ 10 000 (fourth prime ~1 05 10 8).
 * We generate all primes up to 300 000, which yields ~26 000 primes.
 * ----------------------------------------------------------------- */
#define PRIME_TABLE_CAP   30000U          /* enough for our use case */
#define SIEVE_LIMIT       400000U        /* upper bound for primes */

static uint64_t g_primes[PRIME_TABLE_CAP];
static size_t   g_prime_cnt = 0U;
static bool     g_primes_built = false;
static uint64_t g_prefix[PRIME_TABLE_CAP + 1];

/* -----------------------------------------------------------------
 * Comparator for qsort on 64‑bit values.
 * ----------------------------------------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t x = *(const uint64_t *)a;
    const uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

/* -----------------------------------------------------------------
 * Build the prime table once.  Uses a simple sieve of Eratosthenes.
 * ----------------------------------------------------------------- */
static void build_primes(void)
{
    if (g_primes_built)
        return;

    g_primes_built = true;

    static unsigned char sieve[SIEVE_LIMIT + 1];
    size_t i, j;

    /* initialise */
    for (i = 0; i <= SIEVE_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* sieve */
    for (i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 0;

    /* collect primes */
    for (i = 2; i <= SIEVE_LIMIT && g_prime_cnt < PRIME_TABLE_CAP; ++i)
        if (sieve[i])
            g_primes[g_prime_cnt++] = (uint64_t)i;

    /* prefix sums of primes */
    g_prefix[0] = 0;
    for (i = 0; i < g_prime_cnt; ++i)
        g_prefix[i + 1] = g_prefix[i] + g_primes[i];
}

/* -----------------------------------------------------------------
 * Cache for a(n) results up to n = 20 000 (more than enough).
 * ----------------------------------------------------------------- */
#define CACHE_MAX_N 20000U
static seq_t   g_cache[CACHE_MAX_N + 1];
static bool    g_computed[CACHE_MAX_N + 1] = { false };

/* -----------------------------------------------------------------
 * Main function required by the harness.
 * ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build primes once. */
    build_primes();

    /* Guard against zero or too large n. */
    if (n == (idx_t)0 || (size_t)n - 1 >= g_prime_cnt)
        return (seq_t)0;

    /* Cached value? */
    if (n <= (idx_t)CACHE_MAX_N && g_computed[(size_t)n])
        return g_cache[(size_t)n];

    /* Target prime P = prime[n‑1]. */
    const uint64_t target = g_primes[(size_t)n - 1];

    /* Values V_k = prefix[k] - P * k.  k = 0…prime_cnt. */
    static uint64_t v[PRIME_TABLE_CAP + 1];
    size_t i;
    for (i = 0; i <= g_prime_cnt; ++i)
        v[i] = g_prefix[i] - target * (uint64_t)i;

    /* Sort the V_k values. */
    qsort(v, g_prime_cnt + 1, sizeof(uint64_t), cmp_uint64);

    /* Count equal pairs. */
    uint64_t result64 = 0;
    size_t run = 1;
    for (i = 1; i <= g_prime_cnt; ++i) {
        if (v[i] == v[i - 1])
            ++run;
        else {
            result64 += ((uint64_t)run * (uint64_t)(run - 1)) / 2;
            run = 1;
        }
    }
    result64 += ((uint64_t)run * (uint64_t)(run - 1)) / 2;

    /* Cache and return (truncating to seq_t). */
    if (n <= (idx_t)CACHE_MAX_N) {
        g_cache[(size_t)n] = (seq_t)result64;
        g_computed[(size_t)n] = true;
    }
    return (seq_t)result64;
}
