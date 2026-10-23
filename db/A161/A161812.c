/*
 * A161812  Numbers with decreasing but not strictly decreasing prime
 *          signature.
 * https://oeis.org/A161812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 668, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_seq_t, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate all numbers of the form
   2^k1·3^k2·5^k3···p_n^k_n with k1 ≥ k2 ≥ … ≥ k_n
   and at least one equality among consecutive exponents.
   The numbers are generated recursively, stored, sorted and deduplicated.
   The function returns the nth term (1‑based). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utilities provided by the framework */
#include "../utils/compare_seq_t.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

/* --------------------------------------------------------------------- */
/* Static data used by the generator                                    */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* generated numbers */
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static uint64_t *primes   = NULL;  /* list of primes */
static size_t primes_size = 0;
static size_t primes_cap  = 0;

static seq_t max_limit = 0;        /* current generation limit */

/* --------------------------------------------------------------------- */
/* Helper for dynamic arrays                                            */
/* --------------------------------------------------------------------- */
static void grow_cache(void)
{
    if (cache_cap == 0) cache_cap = 16;
    else cache_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
}

static void grow_primes(void)
{
    if (primes_cap == 0) primes_cap = 16;
    else primes_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
}

static void append_cache(seq_t val)
{
    if (cache_size == cache_cap) grow_cache();
    cache[cache_size++] = val;
}

/* Ensure that primes vector has at least (idx+1) primes */
static void ensure_prime_idx(size_t idx)
{
    while (primes_size <= idx)
    {
        uint64_t next;
        if (primes_size == 0)
            next = 2;
        else
            next = next_prime(primes[primes_size - 1]);

        if (primes_cap <= primes_size) grow_primes();
        primes[primes_size++] = next;
    }
}

/* --------------------------------------------------------------------- */
/* Recursive DFS to generate all eligible numbers ≤ max_limit           */
/* --------------------------------------------------------------------- */
static void dfs(size_t pi, int last_exp, seq_t prod,
                bool has_eq, size_t len)
{
    /* Ensure current prime exists */
    ensure_prime_idx(pi);
    const uint64_t prime = primes[pi];

    for (int exp = 1; exp <= last_exp; ++exp)
    {
        seq_t pow_val = pow_uint(prime, (idx_t)exp);
        seq_t new_prod = prod * pow_val;
        if (new_prod > max_limit) break;          /* further exponents only increase */

        bool new_has_eq = has_eq;
        if (len > 0 && exp == last_exp)           /* equality with previous */
            new_has_eq = true;

        size_t new_len = len + 1;
        if (new_len >= 2 && new_has_eq)
            append_cache(new_prod);

        /* Recurse to next prime */
        dfs(pi + 1, exp, new_prod, new_has_eq, new_len);
    }
}

/* --------------------------------------------------------------------- */
/* Generate all values up to a given limit then sort/dedup                */
/* --------------------------------------------------------------------- */
static void generate_up_to(seq_t limit)
{
    cache_size = 0;
    primes_size = 0;
    max_limit = limit;

    dfs(0, 64, (seq_t)1, false, 0);

    if (cache_size > 0)
        qsort(cache, cache_size, sizeof(seq_t), compare_seq_t);

    /* Remove duplicates */
    size_t uniq = 0;
    for (size_t i = 0; i < cache_size; ++i)
    {
        if (uniq == 0 || compare_seq_t(&cache[i], &cache[uniq - 1]) != 0)
            cache[uniq++] = cache[i];
    }
    cache_size = uniq;
}

/* --------------------------------------------------------------------- */
/* Public API                                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* safety for out‑of‑range */

    static seq_t current_limit = 1000000;
    static bool initialized = false;

    if (!initialized || cache_size < (size_t)n)
    {
        seq_t lim = current_limit;
        while (true)
        {
            generate_up_to(lim);
            if (cache_size >= (size_t)n) break;
            lim *= 10;                            /* increase search bound */
        }
        current_limit = lim;
        initialized = true;
    }

    return cache[n - 1];
}
