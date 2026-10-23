/*
 * A156781  Primes : if sums of prime number and 2 consecutive prime numbers
 *          on-left-and-on-right are also primes.
 * https://oeis.org/A156781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A156781.
   a(n) is the n-th prime p_i such that
     p_{i-2} + p_{i-1} + p_i  is prime  AND
     p_i       + p_{i+1} + p_{i+2} is prime.
   The implementation builds a prime table up to a dynamic limit
   and rebuilds it whenever more terms are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/is_prime_big.h"   /* is_prime_big */

static idx_t     prime_limit   = 200000;   /* current sieve bound */
static idx_t   *prime_arr      = NULL;     /* array of primes <= prime_limit */
static size_t    prime_cnt     = 0;        /* number of primes in prime_arr */

static seq_t   *valid          = NULL;     /* sequence values found     */
static size_t    valid_cnt     = 0;        /* number of stored terms    */

static bool      initialized   = false;    /* first rebuild finished    */

/* Rebuild the prime table and recompute all valid terms. */
static void rebuild(void)
{
    /* Free previous data. */
    free(prime_arr);
    free(valid);

    /* Build a fresh list of primes up to prime_limit. */
    build_primes(prime_limit, &prime_arr, &prime_cnt);

    /* Prepare a container for valid terms. */
    size_t cap = prime_cnt / 10 + 10;
    if (cap < 10) cap = 10;
    valid = (seq_t *)malloc(cap * sizeof(seq_t));
    valid_cnt = 0;

    for (size_t i = 2; i + 2 < prime_cnt; ++i) {
        /* Sums involving the current prime.   */
        seq_t left  = (seq_t)(prime_arr[i-2] + prime_arr[i-1] + prime_arr[i]);
        seq_t right = (seq_t)(prime_arr[i] + prime_arr[i+1] + prime_arr[i+2]);

        if (is_prime_big(left) && is_prime_big(right)) {
            /* Grow the valid array if necessary. */
            if (valid_cnt >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(valid, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE); /* abort if realloc fails */
                valid = tmp;
            }
            valid[valid_cnt++] = (seq_t)prime_arr[i];
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure enough terms are available.  Grow the sieve if needed. */
    while (!initialized || valid_cnt < (size_t)n) {
        rebuild();
        if (!initialized)
            initialized = true;      /* first rebuild finished */

        if (valid_cnt >= (size_t)n)
            break;

        /* Increase the prime limit and try again. */
        prime_limit *= 2;
    }

    return valid[(size_t)n - 1];
}
