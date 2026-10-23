/*
 * A161504  Primes congruent to {1, 2, 10, 11, 19, 20} mod 21.
 * https://oeis.org/A161504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime congruent to 1, 2, 10, 11, 19 or 20 (mod 21)
   using a simple primality test. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of primes that satisfy the congruence condition. */
static seq_t *good_primes = NULL;      /* array of qualifying primes */
static size_t good_cnt   = 0;          /* how many primes stored so far */
static size_t good_cap   = 0;          /* current allocated capacity        */
static uint64_t last_checked = 1;      /* last candidate examined           */
static bool initialized = false;      /* indicates that the cache is ready */

/* Allowed residues modulo 21. */
static const uint8_t residues[6] = {1, 2, 10, 11, 19, 20};

/* Check whether a residue is one of the acceptable ones. */
static inline bool matches_remainder(uint8_t r) {
    for (int i = 0; i < 6; ++i) if (r == residues[i]) return true;
    return false;
}

/* Ensure the cache has room for at least one more element. */
static void ensure_good_capacity(void) {
    if (good_cnt >= good_cap) {
        size_t new_cap = good_cap ? good_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(good_primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* abort on allocation failure */
        good_primes = tmp;
        good_cap    = new_cap;
    }
}

/* Return the n‑th term of sequence A161504. */
seq_t a(idx_t n) {
    /* Lazy initialization of the static storage. */
    if (!initialized) {
        good_primes = NULL;
        good_cnt    = 0;
        good_cap    = 0;
        last_checked = 1;          /* will become 2 on first iteration */
        initialized = true;
    }

    /* Generate qualifying primes until we have at least n of them. */
    while (good_cnt < (size_t)n) {
        ++last_checked;                /* candidate prime */

        /* Skip even numbers > 2 quickly. */
        if (last_checked > 2 && (last_checked & 1) == 0) continue;

        /* Primality test.  `is_prime` operates on idx_t (uint64_t). */
        if (!is_prime((idx_t)last_checked)) continue;

        /* Check the congruence condition modulo 21. */
        if (matches_remainder((uint8_t)(last_checked % 21))) {
            ensure_good_capacity();
            good_primes[good_cnt++] = (seq_t)last_checked;
        }
    }

    /* Return the requested element (1‑based index). */
    return good_primes[(size_t)n - 1];
}
