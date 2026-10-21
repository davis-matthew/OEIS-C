/*
 * A055670  a(n) = prime(n) - (-1)^prime(n).
 * https://oeis.org/A055670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = prime(n) - (-1)^(prime(n))
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 9010, u64 n <= 9010,
 *            u128 n <= 9010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 2, this simplifies to a(n) = prime(n) + 1, while a(1) = 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t (*prime_list)[2] = NULL;  /* dynamic array of primes */
static size_t prime_count   = 0;        /* number of primes stored */
static idx_t last_candidate = 2;        /* next candidate to test for primality */

/* Append a new prime to the list, resizing if necessary */
static void append_prime(idx_t p) {
    if (prime_count == 0) {
        prime_list = (idx_t (*)[2])malloc(sizeof(idx_t [2]) * 16);
    } else if (prime_count >= (1U << 29)) { /* guard against overflow */
        /* In practice this will never happen for the sequence size. */
        return;
    } else if (prime_count % 16 == 0) {
        prime_list = (idx_t (*)[2])realloc(prime_list, sizeof(idx_t [2]) * (prime_count + 16));
    }
    (*prime_list)[prime_count++] = p;
}

/* Test if a number is prime using the list of known primes */
static bool is_prime_candidate(idx_t n) {
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (*prime_list)[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate primes up to at least the nth prime */
static void generate_up_to(idx_t n) {
    while (prime_count < (size_t)n) {
        if (last_candidate == 2) {
            append_prime(2);
            last_candidate = 3;
            continue;
        }
        if (last_candidate % 2 == 0) { last_candidate++; continue; }
        if (is_prime_candidate(last_candidate))
            append_prime(last_candidate);
        last_candidate += 2;
    }
}

/* Returns the nth prime (1‑based). */
static idx_t nth_prime(idx_t n) {
    if (n == 0) return 0;          /* not used */
    generate_up_to(n);
    return (*prime_list)[n - 1];
}

/* The requested OEIS sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* outside the sequence */
    if (n == 1) return 1;          /* a(1) = 1 explicitly */
    idx_t p = nth_prime(n);        /* get the nth prime */
    return (seq_t)p + 1;           /* for odd primes, (-1)^p = -1 */
}
