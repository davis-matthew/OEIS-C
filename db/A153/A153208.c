/*
 * A153208  Primes of the form 2*p-1 where p is prime and p-1 is not
 *          squarefree.
 * https://oeis.org/A153208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153208: primes of the form 2*p-1 where p is prime and p-1 is not squarefree */
/* This implementation searches for qualifying primes p, constructs q = 2*p-1,
   checks q for primality, and caches the sequence to answer arbitrary order
   queries efficiently. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_squarefree.h"
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;  /* cached sequence values */
static size_t cache_cnt  = 0;      /* number of values currently stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache        */

static unsigned char *is_prime_arr = NULL;  /* sieve for small numbers          */
static uint32_t *primes_arr        = NULL;  /* list of primes up to sieve_limit  */
static const size_t sieve_limit      = 20000000;  /* 20 million */
static size_t primes_cnt    = 0;           /* number of primes in primes_arr     */
static size_t next_prime_idx = 0;          /* index of next p to examine         */

/* Initialise sieve and prime list */
static void init_sieve(void)
{
    if (is_prime_arr != NULL) return;   /* already initialised */
    is_prime_arr = (unsigned char *)calloc(sieve_limit + 1, sizeof(unsigned char));
    if (!is_prime_arr) abort();
    for (size_t i = 2; i <= sieve_limit; ++i) is_prime_arr[i] = 1;
    size_t sqrt_lim = (size_t)(sqrt((double)sieve_limit) + 1);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                is_prime_arr[j] = 0;
        }
    }
    /* Count primes */
    for (size_t i = 2; i <= sieve_limit; ++i) if (is_prime_arr[i]) ++primes_cnt;
    primes_arr = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes_arr) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (is_prime_arr[i]) primes_arr[idx++] = (uint32_t)i;
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for n=0 */

    /* Ensure cache has enough capacity. */
    if (cache_cap < (size_t)n) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                         (size_t)n * sizeof(seq_t));
        if (!new_cache) abort();
        cache     = new_cache;
        cache_cap = (size_t)n;
    }

    /* Initialise sieve and prime list on first call. */
    if (primes_cnt == 0) init_sieve();

    /* Generate terms until we have at least n values stored. */
    while (cache_cnt < (size_t)n) {
        for (; next_prime_idx < primes_cnt; ++next_prime_idx) {
            uint64_t p = primes_arr[next_prime_idx];
            if (is_squarefree((seq_t)(p - 1))) continue;

            seq_t q = (seq_t)2 * (seq_t)p - (seq_t)1;
            bool prime_q;
            if ((size_t)q <= sieve_limit)
                prime_q = is_prime_arr[(size_t)q];
            else
                prime_q = is_prime_big(q);

            if (prime_q) {
                cache[cache_cnt++] = q;
                ++next_prime_idx;      /* advance to the next prime for next search */
                break;                 /* move to next term requested */
            }
        }
        /* In practice the sieve limit is sufficient for n up to ~10 000. */
        if (next_prime_idx >= primes_cnt)
            abort();  /* unexpected exhaustion of primes */
    }

    return cache[(size_t)n - 1];
}
