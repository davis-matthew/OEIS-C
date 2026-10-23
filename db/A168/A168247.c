/*
 * A168247  Integers of the form (prime(p) - p)/10 for some prime p.
 * https://oeis.org/A168247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (prime(p)-p)/10 for all primes p such that (prime(p)-p)
 *            is a multiple of 10
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"

/* Static storage for the computed sequence terms.  The sequence is guaranteed
 * to need fewer than 20000 terms for n ≤ 10000, so we preallocate a
 * sufficiently large buffer. */
#define MAX_TERMS 20000
static seq_t seq_values[MAX_TERMS];
static idx_t seq_len = 0;

/* Prime list produced by sieve_primes.  prime_vals[0] = 2, prime_vals[1] = 3, ... */
static idx_t *prime_vals = NULL;
static size_t prime_count = 0;

/* Indicates whether the prime table has been generated. */
static bool primes_ready = false;

/* ----- Helper: initialise prime table once ---------------------------------- */
static void init_primes(void)
{
    if (primes_ready)
        return;
    primes_ready = true;

    /* A limit that guarantees enough primes for the required terms.
     * The 570,000‑th prime is 7,485,743, so sieving to 10 000 000
     * yields over 664 000 primes, comfortably covering all needed indices. */
    const idx_t LIMIT = 10000000ULL;
    sieve_primes(LIMIT, &prime_vals, &prime_count);
}

/* ----- Main sequence routine -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; behaviour for n==0 is irrelevant */
        return 0;

    init_primes();

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        if (seq_len >= MAX_TERMS)   /* safety guard */
            break;

        /* If we have examined all available primes, we cannot proceed. */
        static size_t processed_idx = 0;
        if (processed_idx >= prime_count)
            break;

        /* Current prime value p. */
        idx_t p = prime_vals[processed_idx++];

        /* We need the p‑th prime, i.e. prime_vals[p-1].
         * This is valid only if p ≤ prime_count; otherwise the table
         * does not reach that index.  For n up to 10 000 we always
         * stay well within the available range. */
        if (p - 1 >= prime_count)
            break;

        seq_t prime_of_p = (seq_t)prime_vals[p - 1];
        seq_t diff = prime_of_p - (seq_t)p;

        if (diff % 10 == 0) {
            seq_t term = diff / 10;
            seq_values[seq_len++] = term;
        }
    }

    /* If we have enough terms, return the requested one. */
    if (seq_len >= n)
        return seq_values[n - 1];

    /* Insufficient terms: the sequence definition does not provide a value. */
    return 0;
}
