/*
 * A045432  Primes congruent to {3, 4} mod 7.
 * https://oeis.org/A045432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iteratively find primes p such that p ≡ 3 or 4 (mod 7).
   A basic trial‑division test is used with a growing list of known
   primes.  The sequence is stored in a static array, so any order of
   calls is supported. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Forward declarations for static helpers. */
static void add_prime(idx_t p);
static void add_special(idx_t p);
static void ensure_specials(idx_t n);

/* Static state for prime generation. */
static idx_t next_candidate = 2;   /* next integer to test */
static idx_t *prime_list = NULL;   /* list of primes used for division */
static size_t prime_cnt = 0;       /* number of primes in prime_list   */
static size_t prime_cap = 0;       /* allocated capacity of prime_list */

/* Static state for the sequence values. */
static seq_t *specials = NULL;     /* array holding the required primes */
static size_t specials_cnt = 0;     /* number of values stored       */
static size_t specials_cap = 0;     /* allocated capacity            */

/* Append a new prime to prime_list, resizing if necessary. */
static void add_prime(idx_t p) {
    if (prime_cnt >= prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
    }
    prime_list[prime_cnt++] = p;
}

/* Append a new special prime to specials, resizing if necessary. */
static void add_special(idx_t p) {
    if (specials_cnt >= specials_cap) {
        specials_cap = specials_cap ? specials_cap * 2 : 16;
        specials = (seq_t *)realloc(specials, specials_cap * sizeof(seq_t));
    }
    specials[specials_cnt++] = (seq_t)p;
}

/* Simple primality test using the current list of primes. */
static bool is_prime_trial(idx_t n) {
    if (n < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if ((unsigned __int128)p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Ensure that at least n special primes have been generated. */
static void ensure_specials(idx_t n) {
    while (specials_cnt < n) {
        /* Extend prime list up to next_candidate if needed. */
        if (prime_cnt == 0) {
            add_prime(2);
        }
        if (is_prime_trial(next_candidate)) {
            add_prime(next_candidate);
            if ((next_candidate % 7u) == 3u || (next_candidate % 7u) == 4u) {
                add_special(next_candidate);
            }
        }
        ++next_candidate;
    }
}

/* Return the n‑th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0 */
    ensure_specials(n);
    return specials[n - 1];
}
