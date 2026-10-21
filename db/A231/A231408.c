/*
 * A231408  Positive integers that are not the sum of distinct odd primes.
 * https://oeis.org/A231408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A231408: Positive integers that are not the sum of
   distinct odd primes.  Dressler proved that 9 is the largest such
   integer, so the sequence contains exactly the five terms
   1, 2, 4, 6, 9.  The routine builds this list once and then
   returns the requested term. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Maximum number up to which we search for non‑representable integers.
   100 is more than enough because the largest such number is 9. */
#define MAX_SEARCH 100

/* Maximum number of missing integers expected.  Five per the theorem. */
#define MAX_MISS 5

/* Static buffers to hold the computed sequence data. */
static seq_t miss_vals[MAX_MISS];
static size_t miss_count = 0;
static bool initialized = false;

/* Helper: builds the list of odd primes up to MAX_SEARCH. */
static void build_odd_primes(uint32_t **out, size_t *cnt)
{
    bool *is_prime = (bool *)calloc(MAX_SEARCH + 1, sizeof(bool));
    if (!is_prime)
    {
        *out = NULL;
        *cnt = 0;
        return;
    }

    for (int i = 0; i <= MAX_SEARCH; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= MAX_SEARCH; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_SEARCH; j += i)
                is_prime[j] = false;

    /* Count odd primes. */
    size_t primes_cnt = 0;
    for (int i = 3; i <= MAX_SEARCH; i += 2)
        if (is_prime[i])
            ++primes_cnt;

    /* Store them. */
    uint32_t *primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes)
    {
        free(is_prime);
        *out = NULL;
        *cnt = 0;
        return;
    }

    size_t p = 0;
    for (int i = 3; i <= MAX_SEARCH; i += 2)
        if (is_prime[i])
            primes[p++] = (uint32_t)i;

    free(is_prime);
    *out = primes;
    *cnt = primes_cnt;
}

/* Initializes the static data on first call. */
static void init_seq(void)
{
    if (initialized)
        return;

    /* Build odd primes list. */
    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    build_odd_primes(&primes, &primes_cnt);
    if (!primes)
    {
        /* If prime generation failed, leave miss_count 0. */
        initialized = true;
        return;
    }

    /* Subset sum DP to mark representable sums up to MAX_SEARCH. */
    bool *rep = (bool *)calloc(MAX_SEARCH + 1, sizeof(bool));
    if (!rep)
    {
        free(primes);
        initialized = true;
        return;
    }
    rep[0] = true;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        int p = (int)primes[i];
        for (int s = MAX_SEARCH; s >= p; --s)
            if (rep[s - p])
                rep[s] = true;
    }

    /* Collect missing numbers up to MAX_SEARCH. */
    size_t m = 0;
    for (int i = 1; i <= MAX_SEARCH && m < MAX_MISS; ++i)
    {
        if (!rep[i])
        {
            miss_vals[m++] = (seq_t)i;
        }
    }

    miss_count = m;
    free(rep);
    free(primes);
    initialized = true;
}

/* The required function. */
seq_t a(idx_t n)
{
    init_seq();
    if (n == 0 || n > miss_count)
        return (seq_t)0;          /* No term beyond the known ones. */
    return miss_vals[(size_t)n - 1];
}
