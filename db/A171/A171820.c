/*
 * A171820  Numbers n such that the n-th prime is of the form 3k + 1/2 +- 1/2.
 * https://oeis.org/A171820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = the n‑th integer i such that the i‑th prime is not
 *            congruent to 2 modulo 3. The function builds primes
 *            incrementally and records those indices that satisfy the
 *            property, so it returns the correct value for any n≥1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_vals      = NULL;  /* stored sequence values */
static size_t a_cap       = 0;      /* capacity of a_vals   */
static size_t a_len       = 0;      /* number of values     */

static uint64_t *primes   = NULL;  /* list of primes found   */
static size_t primes_cap  = 0;      /* capacity of primes     */
static size_t primes_len  = 0;      /* number of primes      */

static uint64_t next_candidate = 2; /* next integer to test for primality */
static uint64_t prime_index     = 0; /* current prime index (1‑based) */

/* Grow a_vals if necessary.  */
static void ensure_a_cap(void) {
    if (a_len == a_cap) {
        a_cap = a_cap ? a_cap * 2 : 8;
        a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
    }
}

/* Grow primes array if necessary. */
static void ensure_primes_cap(void) {
    if (primes_len == primes_cap) {
        primes_cap = primes_cap ? primes_cap * 2 : 8;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
}

/* Test whether x is prime using previously found primes. */
static bool is_prime(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Compute a(n). */
seq_t a(idx_t n) {

    /* If the value has already been computed, return it. */
    if ((idx_t)n <= a_len) return a_vals[n - 1];

    /* Generate primes and sequence values until we reach the desired n. */
    while ((idx_t)n > a_len) {
        /* Find next prime. */
        while (true) {
            uint64_t cand = next_candidate++;  /* candidate to test */

            if (is_prime(cand)) {
                /* Store the prime. */
                ensure_primes_cap();
                primes[primes_len++] = cand;
                ++prime_index;

                /* Check the property (prime not 2 mod 3). */
                if (cand % 3 != 2) {
                    /* Store the index of this prime. */
                    ensure_a_cap();
                    a_vals[a_len++] = (seq_t)prime_index;
                }
                break; /* move to the next prime */
            }
        }
    }
    return a_vals[n - 1];
}
