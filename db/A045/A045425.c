/*
 * A045425  Primes congruent to {0, 3} mod 7.
 * https://oeis.org/A045425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for primes p such that p % 7 == 0 or p % 7 == 3
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *mod_primes = NULL;      /* primes congruent to 0 or 3 modulo 7 */
static size_t mod_count = 0, mod_cap = 0;

static seq_t *all_primes = NULL;      /* all primes found (used for trial division) */
static size_t all_count = 0, all_cap = 0;

/* Next candidate to test for primality */
static idx_t candidate = 2;

/* Append a prime to the list of all primes */
static void add_all_prime(seq_t p) {
    if (all_count == all_cap) {
        size_t new_cap = all_cap ? (all_cap << 1) : 16;
        all_primes = (seq_t *)realloc(all_primes, new_cap * sizeof(seq_t));
        all_cap = new_cap;
    }
    all_primes[all_count++] = p;
}

/* Append a prime to the mod 7 list (0 or 3) */
static void add_mod_prime(seq_t p) {
    if (mod_count == mod_cap) {
        size_t new_cap = mod_cap ? (mod_cap << 1) : 16;
        mod_primes = (seq_t *)realloc(mod_primes, new_cap * sizeof(seq_t));
        mod_cap = new_cap;
    }
    mod_primes[mod_count++] = p;
}

/* Ensure that we have at least n primes in mod_primes */
static void ensure_mod_primes(idx_t n) {
    if (n == 0) return;
    while (mod_count < (size_t)n) {
        seq_t c = candidate;

        /* primality test by trial division using the list of known primes */
        bool prime = true;
        for (size_t i = 0; i < all_count; ++i) {
            seq_t p = all_primes[i];
            if (p * p > c) break;
            if (c % p == 0) { prime = false; break; }
        }

        if (prime) {
            add_all_prime(c);
            if (c == 7 || c % 7 == 3) {
                add_mod_prime(c);
            }
        }

        /* Move to the next candidate */
        candidate++;
        /* Skip even numbers greater than 2 */
        if (candidate > 3 && (candidate & 1) == 0) candidate++;
    }
}

/* Sequence function as required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_mod_primes(n);
    return mod_primes[n - 1];
}
