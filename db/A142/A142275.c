/*
 * A142275  Primes congruent to 26 mod 43.
 * https://oeis.org/A142275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p mod 43 == 26
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental search, testing each
   number of the form 43*k+26 for primality using the utility
   function is_prime. The results are memoised so repeated calls are
   efficient. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* Helper: dynamic array of seq_t storing the primes found so far. */
static seq_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;
/* The next candidate of the form 43*k + 26 to test.  We initialise
   it to 26 so the first increment gives 69. */
static idx_t next_candidate = 26;

/* Ensure the array has space for at least one more element. */
static void grow_primes_array(void)
{
    size_t new_cap = primes_cap ? primes_cap << 1 : 8;
    seq_t *new_arr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Out of memory – abort. */
        exit(EXIT_FAILURE);
    }
    primes = new_arr;
    primes_cap = new_cap;
}

/* Store a new prime into the memoised array. */
static void store_prime(seq_t p)
{
    if (primes_len >= primes_cap) {
        grow_primes_array();
    }
    primes[primes_len++] = p;
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* behaviour for n < 1 is unspecified; return 0 */
        return (seq_t)0;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        /* Move to the next candidate of the form 43*k+26. */
        do {
            next_candidate += 43;
        } while (next_candidate < 2);    /* skip non‑prime candidates below 2 */

        /* Test primality. */
        if (is_prime(next_candidate)) {
            store_prime((seq_t)next_candidate);
        }
    }

    /* Return the n‑th prime in the sequence (1‑based). */
    return primes[n - 1];
}
