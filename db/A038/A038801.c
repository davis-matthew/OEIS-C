/*
 * A038801  Number of primes less than 10n.
 * https://oeis.org/A038801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(10*n-1) (number of primes < 10n)
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of primes less than 10n for any n ≥ 1.  
   The function keeps a cached sieve up to the maximum limit seen
   so that repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t types. */
static idx_t    sieve_limit = 0;   /* largest n for which the sieve has been built */
static seq_t *prime_counts = NULL; /* prefix counts of primes up to sieve_limit */

/* Build (or rebuild) the sieve up to 'up_to' inclusive.  */
/* After this call, prime_counts[i] == number of primes <= i for all i <= up_to. */
static void build_sieve(idx_t up_to)
{
    /* allocate a boolean array of size up_to+1 */
    size_t len      = (size_t)(up_to + 1);
    bool *is_prime  = (bool *)malloc(len * sizeof(bool));
    if (!is_prime) return;          /* allocation failure -- give up */

    /* initialise */
    for (size_t i = 0; i < len; ++i) is_prime[i] = true;
    if (up_to >= 0) is_prime[0] = false;
    if (up_to >= 1) is_prime[1] = false;

    /* sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= up_to; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= up_to; j += i)
                is_prime[j] = false;
        }
    }

    /* build prefix counts */
    seq_t *counts = (seq_t *)malloc(len * sizeof(seq_t));
    if (!counts) {
        free(is_prime);
        return;                      /* allocation failure */
    }

    seq_t c = 0;
    for (idx_t i = 0; i <= up_to; ++i) {
        if (is_prime[i]) ++c;
        counts[i] = c;
    }

    free(is_prime);

    /* replace previous cache */
    if (prime_counts) free(prime_counts);
    prime_counts  = counts;
    sieve_limit   = up_to;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 */

    idx_t limit = 10 * n - 1;      /* we need primes <= 10n-1 */

    if (limit > sieve_limit)
        build_sieve(limit);
    /* limit <= sieve_limit now, so prime_counts[limit] is valid */
    return prime_counts[limit];
}
