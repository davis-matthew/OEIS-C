/*
 * A074315  Abundant triangular numbers.
 * https://oeis.org/A074315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intersection of A000217 (triangular numbers) and A005101 (abundant numbers).
   a(n) = the nth triangular number T_k (T_k = k(k+1)/2) such that
   σ(T_k) > 2·T_k.  The function searches for sufficient abundant
   triangular numbers and remembers them to answer subsequent calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime sieve -------------------------------------------------- */
static uint32_t *prime_list   = NULL;   /* array of primes          */
static size_t    prime_cnt    = 0;      /* number of primes         */
static int       prime_limit  = 0;      /* sieve limit (maximum p)  */

/* enlarge the prime list up to at least `limit` (inclusive) */
static void ensure_primes(int limit)
{
    if (limit <= prime_limit) return;

    /* simple bit sieve */
    unsigned char *comp = (unsigned char*)calloc(limit + 1, 1);
    if (!comp) exit(EXIT_FAILURE);

    for (int i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (int j = i * i; j <= limit; j += i)
                comp[j] = 1;

    /* count primes */
    size_t new_cnt = 0;
    for (int i = 2; i <= limit; ++i)
        if (!comp[i]) ++new_cnt;

    /* grow array */
    uint32_t *new_list = (uint32_t*)realloc(prime_list, new_cnt * sizeof(uint32_t));
    if (!new_list) exit(EXIT_FAILURE);
    prime_list = new_list;

    size_t idx = 0;
    for (int i = 2; i <= limit; ++i)
        if (!comp[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt  = new_cnt;
    prime_limit = limit;
    free(comp);
}

/* ---------- Sum of divisors --------------------------------------------- */
static seq_t sigma_of(seq_t n)
{
    seq_t sigma = 1;
    seq_t temp  = n;

    /* factorize using stored primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p_u = prime_list[i];
        seq_t p = (seq_t)p_u;

        /* p*p <= temp ? */
        seq_t pp = p * p;
        if (pp > temp) break;

        int exp = 0;
        while (temp % p == 0) {
            temp = temp / p;
            ++exp;
        }

        if (exp > 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            for (int k = 0; k < exp; ++k) {
                p_pow = p_pow * p;
                term  = term + p_pow;
            }
            sigma = sigma * term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        sigma = sigma * (temp + (seq_t)1);
    }

    return sigma;
}

/* ---------- Cache for abundant triangular numbers ----------------------- */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;
static idx_t  last_k       = 0;  /* last triangular index processed */
static seq_t  last_tri     = 0;  /* last triangular value */

/* make sure cache can hold one more element */
static void ensure_cache_cap(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- Main function required by the framework --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Ensure we have enough primes for typical factorisations.
       2 M is ample for all triangular numbers we will examine
       for the required n range (up to about ten thousand terms). */
    ensure_primes(2000000);

    /* Generate abundant triangular numbers until we have `n` of them. */
    while (cache_len < (size_t)n) {
        ++last_k;
        /* T_k = k(k+1)/2, computed in seq_t to avoid overflow. */
        seq_t k_seq   = (seq_t)last_k;
        seq_t t_seq   = k_seq * (k_seq + (seq_t)1) / (seq_t)2;
        last_tri = t_seq;   /* store for future use (not strictly needed) */

        seq_t sigma = sigma_of(t_seq);
        /* Cumulative sum of divisors must exceed 2 * T_k for abundance. */
        if (sigma > (seq_t)2 * t_seq) {
            ensure_cache_cap();
            cache[cache_len++] = t_seq;
        }
    }

    /* Return the requested term (0‑based indexing in the cache). */
    return cache[(size_t)(n - 1)];
}
