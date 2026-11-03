/*
 * A106149  Number of prime factors with multiplicity of the difference
 *          between consecutive primes.
 * https://oeis.org/A106149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = bigomega(p(n+1)-p(n))
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   bigomega_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/bigomega_u64.h"

/* -------------------------------------------------------------------- */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;  /* covers first 6 primes                        */
    double nn    = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint64_t)(bound + 3.0);  /* safety margin                         */
}

/* Static storage for primes */
static idx_t prime_count = 0;          /* number of primes stored          */
static uint64_t *prime_arr = NULL;     /* array of primes (uint64_t)       */

/* Build a sieve up to the smallest bound that contains at least 'need'
   primes.  The function is called only when more primes are required. */
static void build_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    /* Determine an upper bound for the n-th prime. */
    idx_t limit = estimate_prime_upper_bound(need);

    /* Sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes and allocate the array. */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_arr) exit(1);

    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_arr[pos++] = i;

    /* Replace the old array. */
    free(prime_arr);
    prime_arr = new_arr;
    prime_count = cnt;

    free(is_prime);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0, but return 0 safely. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Ensure we have at least n+1 primes (1‑based). */
    build_primes(n + 1);

    /* Compute the difference between consecutive primes. */
    uint64_t diff = prime_arr[n] - prime_arr[n - 1];  /* 0‑based indices */

    return bigomega_u64(diff);
}
