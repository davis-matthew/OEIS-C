/*
 * A103355  a(n) = n - floor( sqrt(prime(n) ) ).
 * https://oeis.org/A103355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - floor( sqrt( prime(n) ) )
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime via a sieve, then subtracts the integer
 * square root of that prime from n.  The function works for any
 * seq_t (ordinary integer or arbitrary‑precision type) and any
 * idx_t (uint64_t).  Auxiliary storage holds the primes in a
 * uint64_t array; values are cast to seq_t only when returned. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/int_sqrt_idx.h"

/* Cached prime array */
static uint64_t *prime_cache = NULL;
static idx_t prime_cache_count = 0;

/* Ensure prime_cache holds at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cache_count)
        return;                       /* already computed */

    /* Estimate an upper bound for the n-th prime */
    idx_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = estimate_prime_upper_bound(n);

    /* Sieve up to 'limit' */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)
        exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes up to limit */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    /* Resize cache to hold all primes up to limit */
    uint64_t *new_cache = (uint64_t *)realloc(prime_cache, count * sizeof(uint64_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    prime_cache = new_cache;
    prime_cache_count = count;

    /* Store primes sequentially */
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            prime_cache[pos++] = (uint64_t)i;
    }

    free(is_prime);
}

/* External sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n < 1; harmless */

    ensure_primes(n);                     /* guarantees prime_cache[n-1] exists */
    uint64_t prime = prime_cache[n - 1];   /* nth prime (1‑based) */
    idx_t root = int_sqrt_idx(prime);      /* floor sqrt of the prime */
    return (seq_t)n - (seq_t)root;        /* compute a(n) in seq_t */
}
