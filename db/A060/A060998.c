/*
 * A060998  Squares of 1 and primes, written backwards.
 * https://oeis.org/A060998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse( x^2 ), where x = 1 if n = 1 else the (n-1)-th
 *            prime
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 3402, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* static storage for prime list */
static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;
static size_t sieve_limit = 0;

/* Ensure that prime_array contains at least 'needed' primes (1‑based index). */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Rough upper bound for the nth prime:  n*(log n + log log n) < n*(20) for n < 1e5 */
    size_t new_limit;
    if (needed < 6)
        new_limit = 15;          /* covers the first 5 primes */
    else
        new_limit = needed * 20 + 100;   /* safety margin */

    /* Allocate boolean array for sieve */
    char *is_prime = (char *)malloc(new_limit + 1);
    if (!is_prime)
        return;  /* allocation failed – unlikely in the required range */

    for (size_t i = 0; i <= new_limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= new_limit; ++i)
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;
        }

    /* Ensure enough space for the primes we will store */
    size_t est_cap = new_limit / 10 + 10;
    if (prime_capacity < est_cap) {
        uint64_t *new_arr = (uint64_t *)realloc(prime_array, est_cap * sizeof(uint64_t));
        if (new_arr) {
            prime_array = new_arr;
            prime_capacity = est_cap;
        }
    }

    /* Fill prime list */
    prime_count = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (is_prime[i]) {
            if (prime_count >= prime_capacity) {
                size_t new_cap = prime_capacity * 2;
                uint64_t *new_arr = (uint64_t *)realloc(prime_array, new_cap * sizeof(uint64_t));
                if (!new_arr)
                    break; /* stop if allocation fails */
                prime_array = new_arr;
                prime_capacity = new_cap;
            }
            prime_array[prime_count++] = i;
        }

    sieve_limit = new_limit;
    free(is_prime);
}

/* Retrieve the k-th prime (k >= 1). */
static uint64_t get_kth_prime(size_t k)
{
    ensure_primes(k);
    return prime_array[k - 1];
}

/* The sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, but returning 0 is harmless */

    seq_t x;
    if (n == 1) {
        x = 1;
    } else {
        /* n-1 is the index in the prime list (1-based). */
        size_t idx = (size_t)n - 1;
        x = (seq_t)get_kth_prime(idx);
    }

    seq_t sq = x * x;          /* square of x */
    return reverse_digits(sq); /* reverse the decimal representation */
}
