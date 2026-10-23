/*
 * A100689  a(n) = prime(n) * 4^prime(n) - 1.
 * https://oeis.org/A100689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * 4^prime(n) - 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based; a(1) = 2*4^2-1 = 31 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helpers from the framework */
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/pow_seq.h"

/* static storage for the primes */
static uint64_t *prime_list = NULL;   /* array of found primes */
static idx_t    prime_count = 0;      /* number of primes stored */

/* ensure that at least n primes are available */
static void ensure_primes(idx_t n)
{
    if (n == 0) return;

    /* If we already have enough primes, just use them */
    if (prime_count >= n) return;

    /* Compute an upper bound that certainly contains the first n primes */
    uint64_t upper = estimate_prime_upper_bound(n);

    /* Allocate a boolean sieve array */
    bool *is_prime = (bool*)malloc((size_t)(upper + 1));
    if (!is_prime) return;          /* allocation failure – give up */

    /* Initialise */
    for (uint64_t i = 0; i <= upper; ++i) is_prime[i] = true;
    if (upper >= 0) is_prime[0] = false;
    if (upper >= 1) is_prime[1] = false;

    /* Eratosthenes sieve */
    for (uint64_t p = 2; p * p <= upper; ++p) {
        if (is_prime[p]) {
            for (uint64_t q = p * p; q <= upper; q += p)
                is_prime[q] = false;
        }
    }

    /* Reallocate the prime list for exactly n elements */
    uint64_t *tmp = (uint64_t*)realloc(prime_list, (size_t)n * sizeof(uint64_t));
    if (!tmp) {        /* realloc failed – restore old state */
        free(is_prime);
        return;
    }
    prime_list = tmp;
    prime_count = 0;

    /* Collect the primes */
    for (uint64_t num = 2; num <= upper && prime_count < n; ++num) {
        if (is_prime[num]) {
            prime_list[prime_count++] = num;
        }
    }

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* undefined for n < 1 */

    /* Make sure we have the nth prime */
    ensure_primes(n);
    if (prime_count < n) return (seq_t)0;    /* safety check */

    uint64_t p = prime_list[n - 1];          /* prime(n) */

    /* Compute 4^prime(n) */
    seq_t power = pow_seq((seq_t)4, (idx_t)p);

    /* Compute p * power - 1 */
    return (seq_t)p * power - (seq_t)1;
}
