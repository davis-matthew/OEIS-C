/*
 * A171024  In the sequence of prime numbers, replace all the '5' digits with
 *          '1' and vice versa.
 * https://oeis.org/A171024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime after swapping decimal digits 1 and 5
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   swap_1_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/swap_1_5.h"

/* Dynamic array of primes discovered so far */
static size_t primes_cap = 0;
static uint64_t *primes_arr = NULL;
static size_t primes_len = 0;
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure the prime array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap : 1;
    while (new_cap < need)
        new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(primes_arr, new_cap * sizeof(uint64_t));
    primes_arr = new_arr;
    primes_cap = new_cap;
}

/* Test whether x is prime, using primes already stored. */
static bool is_prime_candidate(uint64_t x)
{
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes_arr[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Return the n-th term of sequence A171024. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the prime array. */
    if (!primes_arr) {
        primes_arr = (uint64_t *)malloc(16 * sizeof(uint64_t));
        primes_cap = 16;
        primes_len = 0;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_len < (size_t)n) {
        uint64_t cand = next_candidate;
        if (cand < 2)
            cand = 2;
        if (is_prime_candidate(cand)) {
            ensure_capacity(primes_len + 1);
            primes_arr[primes_len++] = cand;
        }
        ++next_candidate;
    }

    /* Retrieve the n-th prime (1‑indexed) and apply the digit swap. */
    uint64_t prime = primes_arr[(size_t)n - 1];
    return (seq_t)swap_1_5(prime);
}
