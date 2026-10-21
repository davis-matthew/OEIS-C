/*
 * A075571  Rearrangement of odd primes such that sum of three successive
 *          terms is composite.
 * https://oeis.org/A075571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 6540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A075571 – rearrangement of odd primes so that every sum of
    three successive terms is composite.
    This implementation constructs the sequence greedily:
    - The first three terms are 3, 5, 7.
    - For n ≥ 3, the next term is the smallest unused odd prime p
      for which a(n‑2)+a(n‑1)+p is composite.
    The function caches computed terms so that arbitrary order of requests
    is handled efficiently.  The algorithm runs in O(n p) time, where p is
    the number of odd primes up to the required limit (≈1 million for
    n≈10 000).  It respects all required limits and works for built‑in
    integer types as well as for the arbitrary‑precision build.   */

/* #includes for the utilities required by the build system */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

static seq_t *cache = NULL;          /* cached sequence values          */
static size_t cache_len = 0;          /* number of values stored         */
static size_t cache_cap = 0;          /* allocated capacity of cache     */

static uint64_t *oddPrimes = NULL;    /* array of odd primes (>2)        */
static size_t oddPrimeCnt = 0;        /* number of odd primes stored     */
static bool *used = NULL;             /* used[p] == true if prime p has been placed */

static const size_t PRIME_LIMIT = 1500000; /* sieve up to 1.5M (≈118k odd primes) */

/* -------------------------------------------------------------- */
/*  Ensure cache has at least 'need' capacity                       */
static void cache_ensure(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------- */
/*  Build odd primes up to PRIME_LIMIT using a simple sieve         */
static void build_odd_primes(void)
{
    /* Allocate and initialize boolean array */
    bool *is_pr = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_pr)
        return; /* allocation failure – unlikely in this context */

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        is_pr[i] = true;

    for (size_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (is_pr[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                is_pr[(size_t)j] = false;
        }
    }

    /* Count odd primes (>2) to allocate array */
    size_t cnt = 0;
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2)
        if (is_pr[i])
            ++cnt;

    oddPrimes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    oddPrimeCnt = cnt;
    size_t idx = 0;
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2)
        if (is_pr[i])
            oddPrimes[idx++] = (uint64_t)i;

    used = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));

    free(is_pr);
}

/* -------------------------------------------------------------- */
/*  Return a(n) with memoization                                   */
seq_t a(idx_t n)
{
    /* First time: initialise cache and prime tables */
    if (cache_len == 0) {
        build_odd_primes();
        /* Reserve space for the first few terms */
        cache_ensure(8);
        /* Initial terms are 3, 5, 7 (the smallest odd primes) */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)7;
        cache_len = 3;
        used[3] = true;
        used[5] = true;
        used[7] = true;
    }

    /* If requested term is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        /* Sum of the two previous terms */
        idx_t last2 = (cache_len >= 2) ? (idx_t)cache[cache_len - 2] : 0;
        idx_t last1 = (cache_len >= 1) ? (idx_t)cache[cache_len - 1] : 0;
        idx_t baseSum = last2 + last1;

        /* Find the smallest unused odd prime with composite sum */
        uint64_t candidate = 0;
        for (size_t i = 0; i < oddPrimeCnt; ++i) {
            uint64_t p = oddPrimes[i];
            if (used[(size_t)p])
                continue;
            idx_t s = baseSum + p;
            if (!is_prime(s)) {
                candidate = p;
                break;
            }
        }

        /* Safety check – unlikely to fail with the chosen limit */
        if (candidate == 0)
            return (seq_t)0;

        /* Store the candidate */
        cache_ensure(cache_len + 1);
        cache[cache_len] = (seq_t)candidate;
        used[(size_t)candidate] = true;
        ++cache_len;
    }

    return cache[(size_t)n];
}
