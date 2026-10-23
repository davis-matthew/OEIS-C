/*
 * A116579  a(1) = a(2) = 1; a(n) = floor(prime(n)/6) for n > 2.
 * https://oeis.org/A116579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(1)=a(2)=1; for n>2 a(n)=floor(prime(n)/6)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime on demand and caches the results.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached primes are stored as 32‑bit integers; the 10 000th prime
   is 104 729, which fits easily.  The cache expands on demand. */
static uint32_t *prime_cache   = NULL;  /* array of primes */
static size_t   prime_cap     = 0;      /* current allocation size */
static size_t   prime_cnt     = 0;      /* number of primes stored */

/* Ensure the cache contains at least `n_needed` primes.  The routine
   appends new primes by trial division using the already cached ones. */
static void ensure_prime_cache(size_t n_needed)
{
    if (prime_cnt >= n_needed)
        return;

    /* grow the array as needed */
    if (prime_cache == NULL) {
        prime_cap = 16;
        prime_cache = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_cache) exit(EXIT_FAILURE);  /* allocation failure */
    }
    else if (n_needed > prime_cap) {
        while (prime_cap < n_needed)
            prime_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                            prime_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
    }

    /* Add the first prime if the cache is empty */
    if (prime_cnt == 0) {
        prime_cache[prime_cnt++] = 2;
    }

    /* Candidate starts at 3, then only odd numbers */
    uint32_t candidate = (prime_cnt == 1) ? 3
                                          : prime_cache[prime_cnt - 1] + 2;

    while (prime_cnt < n_needed) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            prime_cache[prime_cnt++] = candidate;
        candidate += 2;  /* stay on odd numbers */
    }
}

/* Return the nth prime (1‑based).  The cache is extended if necessary. */
static uint32_t nth_prime(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined, but never called */
    ensure_prime_cache((size_t)n);
    return prime_cache[n - 1];
}

/* The sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n <= 2)                         /* a(1)=a(2)=1 */
        return (seq_t)1;

    uint32_t p = nth_prime((size_t)n);  /* nth prime fits in 32 bits */
    return (seq_t)(p / 6);              /* floor division */
}
