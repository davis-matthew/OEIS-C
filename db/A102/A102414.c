/*
 * A102414  Smallest semiprime greater than n-th prime.
 * https://oeis.org/A102414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A102414
 *  Definition: a(n) = the smallest semiprime greater than the n‑th prime p_n
 *  (a semiprime is a positive integer that is a product of exactly two primes,
 *   not necessarily distinct).
 *
 *  The implementation builds a sieve of Eratosthenes once to obtain all
 *  primes up to 200 000 – more than enough for the first 10 000 primes –
 *  and then for each request finds the next semiprime by trial division.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------
 *  Prime sieve (static state)
 * ---------------------------------------------------------------------- */
#define SIEVE_LIMIT 200000ULL   /* covers >10 000 primes */

static bool primes_initialized = false;
static uint64_t *prime_list = NULL;   /* array of primes (uint64_t) */
static idx_t prime_count = 0;         /* number of primes stored */

/* Build the prime list once
 * The caller must ensure SIEVE_LIMIT is large enough for the requested n. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    const uint64_t limit = SIEVE_LIMIT;
    /* Allocate a boolean array for the sieve */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime)
        abort();

    for (uint64_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes to allocate the array */
    idx_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(is_prime);
        abort();
    }

    /* Store primes */
    idx_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    prime_count = cnt;
    free(is_prime);
    primes_initialized = true;
}

/* ----------------------------------------------------------------------
 *  Test whether a number is semiprime
 *  Uses the precomputed prime list.  The number is assumed to fit in
 *  uint64_t; a failure to detect a semiprime due to missing larger primes
 *  cannot happen for the range we support.
 * ---------------------------------------------------------------------- */
static bool is_semiprime(uint64_t n)
{
    uint64_t temp = n;
    unsigned factor_cnt = 0;

    for (idx_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp)
            break;

        while (temp % p == 0) {
            ++factor_cnt;
            temp /= p;
            if (factor_cnt > 2)
                return false;
        }
    }

    if (temp > 1)          /* remaining prime factor > sqrt(n) */
        ++factor_cnt;

    return factor_cnt == 2;
}

/* ----------------------------------------------------------------------
 *  Public function: compute a(n)
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n<1; return 0 as a guard */
        return (seq_t)0;

    init_primes();

    if (n > prime_count)  /* caller requested beyond precomputed range */
        abort();

    uint64_t p_n = prime_list[n - 1];     /* nth prime (1‑based) */
    uint64_t candidate = p_n + 1;

    while (!is_semiprime(candidate))
        ++candidate;

    return (seq_t)candidate;
}
