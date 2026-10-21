/*
 * A211005  Pair (i, j) where i = number of adjacent nonprimes and j = number
 *          of adjacent primes.
 * https://oeis.org/A211005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n‑th run of consecutive integers having the
 *            same prime status (prime or composite), starting with the
 *            integer 1 as the first non‑prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIMIT 2000000U   /* enough for n up to about 10000 */

/* Prime sieve up to SIEVE_LIMIT */
static bool *prime_sieve = NULL;

/* Build a classical Sieve of Eratosthenes up to SIEVE_LIMIT. */
static void init_sieve(void)
{
    prime_sieve = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!prime_sieve) return;
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) prime_sieve[i] = true;
    if (SIEVE_LIMIT >= 0) { prime_sieve[0] = false; }
    if (SIEVE_LIMIT >= 1) { prime_sieve[1] = false; }
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_sieve[j] = false;
        }
    }
}

/* Dynamic array for cached run lengths */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;   /* number of runs stored */
static size_t cache_cap    = 0;   /* capacity of the cache array */

/* State for run length generation */
static idx_t  next_number      = 1;   /* next natural number to process */
static int    current_status   = -1;  /* -1 = no run started yet, 0 = nonprime, 1 = prime */
static size_t run_len          = 0;   /* length of the current run */
static bool   sieve_inited     = false;

/* Ensure that the cache contains at least n run lengths. */
static void ensure_cache_upto(idx_t n)
{
    if (!sieve_inited) {
        init_sieve();
        sieve_inited = true;
    }

    while (cache_len < (size_t)n) {
        int status;
        if (next_number <= SIEVE_LIMIT) {
            status = prime_sieve[next_number] ? 1 : 0;
        } else {
            /* Fallback: fall back to simple primality test when beyond sieve. */
            status = 1;
            /* naive test; this should never happen for the required n. */
            for (idx_t d = 2; d * d <= next_number; ++d)
                if (next_number % d == 0) { status = 0; break; }
        }

        if (current_status == -1) {
            /* First number processed */
            current_status = status;
            run_len = 1;
        } else if (status == current_status) {
            ++run_len;
        } else {
            /* Status changed – finish previous run */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)run_len;
            current_status = status;
            run_len = 1;
        }
        ++next_number;
    }
}

/* Public function returning a(n).  a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset doesn't matter */
    ensure_cache_upto(n);
    return cache[n - 1];
}
