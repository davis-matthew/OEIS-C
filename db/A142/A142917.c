/*
 * A142917  Primes congruent to 50 mod 63.
 * https://oeis.org/A142917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 50 modulo 63 (OEIS A142917)
 * Range:     u8 n <= 2, u16 n <= 156, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;     /* array of found primes */
static size_t primes_count = 0;  /* number of primes stored */
static size_t primes_cap = 0;    /* current capacity of the array */
static seq_t next_candidate = 0; /* next number to test (always ≡ 50 mod 63) */

static void ensure_primes(idx_t n) {
    if (primes == NULL) {
        /* initial allocation */
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        next_candidate = (seq_t)113; /* first prime ≡ 50 (mod 63) */
    }
    while (primes_count < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            if (primes_count >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real scenario we would handle the error, but for
                       this environment it's safe to abort. */
                    abort();
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_count++] = next_candidate;
        }
        next_candidate += (seq_t)63;
    }
}

seq_t a(idx_t n) {
    ensure_primes(n);
    return primes[n - 1];
}
