/*
 * A090455  Difference between numbers of binary 1's of n and binary 1's of
 *          n-th prime.
 * https://oeis.org/A090455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = popcount(n) – popcount(n‑th prime)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   popcount_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/popcount_ull.h"

/* Static storage for primes to support repeated calls */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* Extend the prime table until it contains at least n primes.
   Returns the nth prime (1‑based). */
static uint64_t get_nth_prime(idx_t n)
{
    if (n == 0)          /* undefined, but keep safe */
        return 0;

    /* Initialise the list with the first prime */
    if (prime_list == NULL) {
        prime_capacity = 16;
        prime_list = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_count = 1;
    }

    /* Extend the list if necessary */
    while (prime_count < (size_t)n) {
        /* Grow the table if full */
        if (prime_capacity <= prime_count) {
            size_t new_cap = prime_capacity * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!tmp) { /* allocation failure – abort */
                abort();
            }
            prime_list = tmp;
            prime_capacity = new_cap;
        }

        /* Candidate for the next prime: odd number after the last prime */
        uint64_t cand = (prime_count == 1) ? 3 : prime_list[prime_count - 1] + 2;

        while (1) {
            int isprime = 1;
            uint64_t limit = (uint64_t)sqrt((double)cand);
            for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
                if (cand % prime_list[i] == 0) {
                    isprime = 0;
                    break;
                }
            }
            if (isprime) {
                prime_list[prime_count++] = cand;
                break;
            }
            cand += 2;  /* skip even numbers */
        }
    }

    return prime_list[n - 1];  /* 1‑based indexing */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* values below the offset are irrelevant */
        return 0;

    seq_t pc_n = popcount_ull((uint64_t)n);
    uint64_t pn   = get_nth_prime(n);
    seq_t pc_p = popcount_ull(pn);

    return pc_n - pc_p;
}
