/*
 * A101301  The sum of the first n primes, minus n.
 * https://oeis.org/A101301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (prime(k) - 1)
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for generated primes */
static uint64_t *prime_arr    = NULL;  /* array of primes (uint64_t) */
static idx_t     prime_count  = 0;     /* number of primes stored */
static idx_t     prime_cap    = 0;     /* allocated capacity of prime_arr */
static uint64_t  next_candidate = 2;   /* next odd candidate to test */

/* Ensure the global array contains at least n primes */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    /* Grow the array when needed */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    while (prime_count < n) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        /* Trial division by known primes up to sqrt(cand) */
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the prime */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_count++] = cand;
        }

        /* Update next_candidate: skip even numbers after 2 */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* Public function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset begins at 1; undefined behaviour for n < 1 */
        return 0;

    ensure_n_primes(n);

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
        sum += (seq_t)(prime_arr[i] - 1);

    return sum;
}
