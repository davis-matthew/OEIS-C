/*
 * A168476  Numbers that are the product of two distinct primes and they are
 *          partial sum of products of two distinct primes.
 * https://oeis.org/A168476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168476: numbers that are the product of two distinct primes
   and that themselves are partial sums of such products. */

/* The algorithm generates semiprimes in increasing order, keeps a running
   partial sum, and records those sums that are also square‑free semiprimes.
   For arbitrary order of calls it uses a static cache that is expanded
   lazily. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* ---- Helper predicates ------------------------------------------- */

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)   return false;
    return true;
}

static inline uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return i;
    return n;     /* n is prime */
}

static inline bool semiprime_distinct_uint64(uint64_t n)
{
    if (n < 6)                   return false;      /* 2*3 is the smallest one */
    uint64_t p = smallest_prime_factor_uint64(n);
    if (p == n)                  return false;      /* prime */
    uint64_t q = n / p;
    if (p == q)                  return false;      /* square of a prime */
    return is_prime_uint64(q);
}

/* ---- Cache and generation ---------------------------------------- */

static seq_t    *cache      = NULL;   /* cache for a(n) values       */
static size_t   cache_cnt  = 0;       /* number of cached terms      */
static size_t   cache_cap  = 0;       /* allocated capacity          */
static uint64_t next_semiprime = 6;   /* next integer to test       */
static uint64_t sum64        = 0;     /* running partial sum         */

/* Ensure the cache has at least 'needed' elements. */
static void ensure_terms(idx_t needed)
{
    size_t need = needed;
    if (cache_cnt >= need) return;

    /* Allocate or enlarge the cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < need) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have 'need' of them. */
    while (cache_cnt < need) {
        /* Find the next distinct semiprime. */
        while (!semiprime_distinct_uint64(next_semiprime))
            ++next_semiprime;

        /* Add it to the running partial sum. */
        if (sum64 > UINT64_MAX - next_semiprime)   /* overflow guard */
            return;
        sum64 += next_semiprime;

        /* If the new sum itself is a distinct semiprime, cache it. */
        if (semiprime_distinct_uint64(sum64))
            cache[cache_cnt++] = (seq_t)sum64;

        ++next_semiprime;   /* advance to check the next candidate */
    }
}

/* ---- Public sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, return 0 */
    ensure_terms(n);
    return cache[n - 1];
}
