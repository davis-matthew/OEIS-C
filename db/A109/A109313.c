/*
 * A109313  Difference between prime factors of n-th semiprime.
 * https://oeis.org/A109313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |q - p|, where sp(n) = p * q is the n‑th semiprime *
 *            (with p ≤ q). a(n) = 0 if sp(n) is a square of a prime
 * Range:     u8 n <= 161, u16 n <= 9241, u32 n <= 9241, u64 n <= 9241,
 *            u128 n <= 9241, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

/* forward declaration of helper compare function */
static int cmp_uint64(const void *a, const void *b);

/* static storage for primes and semiprimes */
static uint32_t *primes = NULL;
static size_t    primes_cnt   = 0;
static size_t    primes_cap   = 0;

static uint64_t *semiprimes = NULL;
static size_t    semiprimes_cnt   = 0;
static size_t    semiprimes_cap   = 0;

static uint64_t current_limit = 0;   /* limit up to which primes were sieved */

/* allocate and ensure capacity for an array of uint32_t */
static void ensure_prime_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1024;
        while (new_cap < needed) new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();            /* memory error */
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* allocate and ensure capacity for an array of uint64_t */
static void ensure_semiprime_cap(size_t needed)
{
    if (semiprimes_cap < needed) {
        size_t new_cap = semiprimes_cap ? semiprimes_cap * 2 : 1024;
        while (new_cap < needed) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(semiprimes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        semiprimes = tmp;
        semiprimes_cap = new_cap;
    }
}

/* simple sieve of Eratosthenes up to 'limit', storing primes in 'primes' */
static void sieve_primes(uint64_t limit)
{
    size_t sz = (size_t)(limit / 2) + 1;   /* skip evens except 2 */
    uint8_t *is_comp = (uint8_t *)calloc(sz, 1);
    if (!is_comp) abort();

    size_t max_i = (size_t)(sqrt((double)limit) / 2);
    for (size_t i = 1; i <= max_i; ++i) {
        if (!is_comp[i]) {
            uint64_t p = 2 * i + 1;
            for (uint64_t j = p * p; j <= limit; j += 2 * p)
                is_comp[(j - 1) / 2] = 1;
        }
    }

    /* 2 is prime */
    ensure_prime_cap(primes_cnt + 1);
    primes[primes_cnt++] = 2;

    for (size_t i = 1; i <= (limit - 1) / 2; ++i) {
        if (!is_comp[i]) {
            ensure_prime_cap(primes_cnt + 1);
            primes[primes_cnt++] = (uint32_t)(2 * i + 1);
        }
    }
    free(is_comp);
}

/* Ensure we have primes up to 'limit'. Extend limit if necessary. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= current_limit) return;
    /* resieve from scratch for simplicity */
    free(primes);
    primes = NULL;
    primes_cnt = 0;
    primes_cap = 0;
    sieve_primes(limit);
    current_limit = limit;
}

/* Generate all semiprimes <= limit, sorted ascending. */
static void generate_semiprimes_up_to(uint64_t limit)
{
    ensure_primes_up_to(limit);

    /* Reserve space for expected count: crude upper bound */
    size_t est = (size_t)(((double)limit) * (log(log((double)limit)) / log((double)limit)) * 1.5 + 1000);
    ensure_semiprime_cap(est);

    size_t k = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > limit) break;
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t q = primes[j];
            unsigned __int128 prod128 = (unsigned __int128)p * q;
            if (prod128 > limit) break;
            ensure_semiprime_cap(k + 1);
            semiprimes[k++] = (uint64_t)prod128;
        }
    }
    semiprimes_cnt = k;

    /* sort the array */
    qsort(semiprimes, semiprimes_cnt, sizeof(uint64_t), cmp_uint64);
}

/* Compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Find the difference of prime factors of a semiprime into two primes. */
static seq_t factor_difference(uint64_t sp)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > sp) break;
        if (sp % p == 0) {
            uint64_t q = sp / p;
            uint64_t diff = q - p;
            return (seq_t)diff;
        }
    }
    /* If not found, then sp is a square of a prime > sqrt(limit) */
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined, return 0 */

    /* ensure we have at least n semiprimes */
    while (semiprimes_cnt < (size_t)n) {
        /* increase limit heuristically: start at 1e6 or double current limit */
        uint64_t new_limit = current_limit ? current_limit * 2 : 1000000;
        generate_semiprimes_up_to(new_limit);
    }

    uint64_t sp = semiprimes[n - 1];              /* n-th semiprime (offset 1) */
    return factor_difference(sp);
}
