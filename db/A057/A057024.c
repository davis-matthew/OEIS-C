/*
 * A057024  Largest odd factor of (n-th prime+1); k when n-th prime is written
 *          as k*2^m-1 [with k odd].
 * https://oeis.org/A057024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest odd factor of (prime(n)+1)
 * Range:     u8 n <= 54, u16 n <= 7765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: ensure we have at least n primes stored */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;
static uint64_t last_checked = 1;

/* Allocate or grow the prime list */
static void ensure_capacity(void) {
    if (prime_count >= prime_capacity) {
        size_t new_capacity = prime_capacity == 0 ? 64 : prime_capacity * 2;
        uint64_t *new_list = (uint64_t *)realloc(prime_list, new_capacity * sizeof(uint64_t));
        if (!new_list) exit(1); /* allocation failed */
        prime_list = new_list;
        prime_capacity = new_capacity;
    }
}

/* Triangular simple primality test using stored primes.
   Assumes prime_list is already populated up to sqrt(candidate). */
static bool is_prime_candidate(uint64_t candidate) {
    if (candidate < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Returns the n-th prime (1‑based) */
static uint64_t nth_prime(idx_t n) {
    if (n < 1) return 0; /* invalid, but sequence is 1‑based */
    /* Ensure at least one prime (2) */
    if (prime_count == 0) {
        ensure_capacity();
        prime_list[0] = 2;
        prime_count = 1;
        last_checked = 2;
    }
    /* Generate primes until we have n of them */
    while (prime_count < (size_t)n) {
        last_checked += 1;
        if ((last_checked & 1) == 0) last_checked++; /* skip even numbers */
        if (is_prime_candidate(last_checked)) {
            ensure_capacity();
            prime_list[prime_count++] = last_checked;
        }
    }
    return prime_list[(size_t)n - 1];
}

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t p = nth_prime(n);
    seq_t val = (seq_t)(p + 1);
    while ((val & (seq_t)1) == 0) val >>= 1;
    return val;
}
