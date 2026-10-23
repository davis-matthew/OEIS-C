/*
 * A140475  1 along with primes greater than 3.
 * https://oeis.org/A140475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1, a(n) = the (n+1)-th prime for n > 1
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches all primes up to the required index using a
   simple incrementally growing Sieve of Eratosthenes. */
/* It is fully C11/C++17 compliant and works with any seq_t definition
   (unsigned integer or arbitrary‑precision). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached list of primes (uniformly uint64_t).  It is safe to convert
   these values to seq_t because all primes we need fit in 64 bits. */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_count = 0;       /* number of primes stored */
static idx_t    prime_limit  = 0;       /* highest number sieved */

/* Allocate and sieve all primes up to 'limit', replacing the previous cache. */
static void sieve_up_to(idx_t limit)
{
    /* Free old cache */
    free(prime_cache);

    size_t lim = (size_t)limit;
    /* Allocate boolean array: 1 => prime, 0 => composite */
    char *is_prime = (char *)calloc(lim + 1, 1);
    if (!is_prime) abort();                      /* allocation failure */

    /* Mark all numbers >=2 as potential primes */
    for (size_t i = 2; i <= lim; ++i)
        is_prime[i] = 1;

    /* Sieve */
    for (size_t p = 2; p * p <= lim; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= lim; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count primes to allocate the result array */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate array of primes */
    uint64_t *new_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_arr) abort();                      /* allocation failure */

    /* Fill the array */
    size_t j = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i])
            new_arr[j++] = (uint64_t)i;

    free(is_prime);

    prime_cache = new_arr;
    prime_count = cnt;
    prime_limit = limit;
}

/* Ensure that at least 'needed' primes are cached. */
static void extend_primes_to_count(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Start from a reasonable initial limit and double until we have enough. */
    idx_t limit = (prime_limit > 0) ? prime_limit * 2 : 20;

    while (prime_count < needed) {
        sieve_up_to(limit);
        if (prime_count >= needed)
            break;
        limit *= 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                     /* the special first term */

    /* For n > 1 we need the (n+1)-th prime (0‑based index n). */
    size_t desired_index = (size_t)(n + 1);   /* number of primes needed */
    extend_primes_to_count(desired_index);

    return (seq_t)prime_cache[desired_index - 1]; /* zero‑based indexing */
}
