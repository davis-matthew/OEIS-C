/*
 * A053144  Cototient of the n-th primorial number.
 * https://oeis.org/A053144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n) – φ(A002110(n)), * where A002110(n) is the
 *            n-th primorial Qₙ = p₁×p₂×…×pₙ * and φ denotes Euler's totient.
 *            * * Recurrence used: * a(1) = 1 * For n > 1: * let pₙ be the
 *            n‑th prime, * let Qₙ₋₁ be the primorial of the first n–1 primes,
 *            * and let a(n–1) be the previous term. * Then...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_capacity, next_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/next_prime_uint64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the (unrequested) offset n = 0 safely. */
    if (n == 0) return (seq_t)0;

    /* Static storage for memoisation. */
    static seq_t *values      = NULL;   /* values[0] = a(1), etc. */
    static size_t values_cap  = 0;
    static size_t values_len  = 0;

    static uint64_t *primes   = NULL;   /* primes[0] = 2, primes[i] = p_{i+1}. */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    static seq_t primorial   = 0;       /* Qₙ for the last computed n. */

    /* Initial setup for the first term. */
    if (values_len == 0) {
        /* Allocate space for a(1). */
        ensure_capacity(1, &values_cap, (void **)&values, sizeof(seq_t));
        values[0]            = (seq_t)1;          /* a(1) = 1. */
        values_len           = 1;
        primorial           = (seq_t)2;           /* Q₁ = 2. */

        /* Allocate space for the first prime. */
        ensure_capacity(1, &prime_cap, (void **)&primes, sizeof(uint64_t));
        primes[0]            = 2;                /* p₁ = 2. */
        prime_len            = 1;
    }

    /* Extend the cache up to the requested index n. */
    while (values_len < n) {
        /* Ensure we have the next prime pₙ. */
        if (prime_len <= values_len) {
            uint64_t p = next_prime_uint64(primes[prime_len - 1], primes, prime_len);
            ensure_capacity(prime_len + 1, &prime_cap, (void **)&primes, sizeof(uint64_t));
            primes[prime_len] = p;
            prime_len++;
        }
        uint64_t p = primes[values_len];   /* pₙ, where values_len = n‑1. */

        /* Compute the new term using the recurrence. */
        seq_t a_prev   = values[values_len - 1];
        seq_t new_a    = a_prev * ((seq_t)(p - 1)) + primorial;
        seq_t new_q    = primorial * (seq_t)p;  /* Qₙ  = Qₙ₋₁ · p. */

        ensure_capacity(values_len + 1, &values_cap, (void **)&values, sizeof(seq_t));
        values[values_len] = new_a;
        values_len++;

        primorial = new_q;
    }

    return values[n - 1];
}
