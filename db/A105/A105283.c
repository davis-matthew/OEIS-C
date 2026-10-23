/*
 * A105283  (2n)-th prime mod n.
 * https://oeis.org/A105283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p_{2n} mod n
 * Where p_k is the k-th prime.
 *
 * The implementation builds a list of primes large enough to supply
 * the 2*n-th prime on demand.  It relies on estimate_prime_upper_bound()
 * (in ../utils/estimate_prime_upper_bound.h) to get a conservative upper
 * bound for the n-th prime.  The array of primes is rebuilt whenever a
 * larger index is requested.  The function works for any seq_t and
 * idx_t supplied by the build environment.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

/* static storage for primes */
static uint32_t *prime_arr  = NULL;  /* array of primes */
static idx_t    prime_count = 0;     /* number of primes stored */

/* Build the list of primes up to at least 'need' primes */
static void
build_primes(idx_t need)
{
    /* free previous array if any */
    if (prime_arr) {
        free(prime_arr);
        prime_arr = NULL;
        prime_count = 0;
    }

    /* compute an upper bound for the needed prime */
    idx_t limit = estimate_prime_upper_bound(need);
    if (limit < 2) limit = 2;  /* safety for tiny n */

    /* allocate boolean sieve */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    /* initialize sieve */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    /* sieve */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    /* allocate array for primes */
    prime_arr = (uint32_t *)malloc((size_t)count * sizeof(uint32_t));
    if (!prime_arr) exit(EXIT_FAILURE);

    /* fill prime array */
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit && idx < count; ++i)
        if (is_prime[i]) prime_arr[idx++] = (uint32_t)i;

    prime_count = count;

    /* cleanup */
    free(is_prime);
}

/* Main function to compute the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */

    /* index of prime required: 2*n */
    idx_t prime_idx = 2 * n;

    /* Ensure we have enough primes */
    while (prime_count < prime_idx) {
        build_primes(prime_idx);
        /* If estimate was too low, double the requirement */
        if (prime_count < prime_idx) prime_idx *= 2;
    }

    /* p_{2n} is the prime at index prime_idx-1 (0‑based) */
    uint32_t prime = prime_arr[prime_idx - 1];

    seq_t prime_seq = (seq_t)prime;
    seq_t n_seq     = (seq_t)n;
    return prime_seq % n_seq;
}
