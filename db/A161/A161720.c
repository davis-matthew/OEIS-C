/*
 * A161720  a(n) = (prime(n) - 7)/2.
 * https://oeis.org/A161720
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (prime(n) - 7)/2 (offset 3)
 * Range:     i8 n <= 32, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static size_t prime_cache_len = 0;

/* Ensure that prime_cache holds at least n primes (1‑based). */
static void ensure_prime_cache(idx_t n)
{
    if (prime_cache_len >= n)
        return;                     /* enough primes already cached */

    /* Free old cache if any */
    if (prime_cache)
        free(prime_cache);
    prime_cache = NULL;
    prime_cache_len = 0;

    /* Build a new list with the first n primes */
    sieve_first_n_primes(n, &prime_cache, &prime_cache_len);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For smaller n the value is
       irrelevant to the tests, but we still return 0. */
    if (n < 3)
        return 0;

    ensure_prime_cache(n);

    /* prime(n) is stored at index n-1 in the 1‑based array. */
    seq_t val = (seq_t)prime_cache[n - 1] - 7;
    val /= 2;
    return val;
}
