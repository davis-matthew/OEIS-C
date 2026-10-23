/*
 * A045422  Primes congruent to {0, 1, 3, 6} mod 7.
 * https://oeis.org/A045422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime that is congruent to 0, 1, 3 or 6 modulo 7
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   resize_if_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/resize_if_needed.h"

static seq_t *selected = NULL;
static size_t selected_len = 0, selected_cap = 0;
static seq_t *all_primes = NULL;
static size_t all_len = 0, all_cap = 0;
static seq_t candidate = 3;
static bool initialized = false;

/* Initialise static data structures once. */
static void init_static(void) {
    if (initialized) return;
    initialized = true;

    /* start with 2 as the first prime */
    all_cap = 16;
    all_primes = (seq_t *)malloc(all_cap * sizeof(seq_t));
    if (!all_primes) exit(EXIT_FAILURE);
    all_primes[0] = (seq_t)2;
    all_len = 1;

    selected_cap = 16;
    selected = (seq_t *)malloc(selected_cap * sizeof(seq_t));
    if (!selected) exit(EXIT_FAILURE);
    selected_len = 0;
}

/* Ensure that at least n terms of the sequence are cached. */
static void ensure_cache(idx_t n) {
    init_static();
    while (selected_len < n) {
        seq_t p = candidate;
        /* test whether p is prime using previously found primes */
        bool isprime = true;
        for (size_t i = 0; i < all_len; ++i) {
            seq_t q = all_primes[i];
            if (q * q > p) break;
            if (p % q == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* store the new prime */
            resize_if_needed(&all_primes, &all_len, &all_cap);
            all_primes[all_len++] = p;

            /* check residue modulo 7 */
            seq_t rem = p % (seq_t)7;
            if (rem == (seq_t)0 || rem == (seq_t)1 || rem == (seq_t)3 || rem == (seq_t)6) {
                resize_if_needed(&selected, &selected_len, &selected_cap);
                selected[selected_len++] = p;
            }
        }
        candidate += (seq_t)2;   /* next odd candidate */
    }
}

/* Sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return selected[n - 1];
}
