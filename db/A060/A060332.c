/*
 * A060332  Primes the sum of seven consecutive composite numbers.
 * https://oeis.org/A060332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 810, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via a sliding window over composite numbers and primality tests. */
/* Each prime in the sequence is the sum of seven consecutive composite integers. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Grow a dynamically allocated array of seq_t elements. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t new_len) {
    if (new_len <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap;
    while (new_cap < new_len) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Return the nth term of OEIS sequence A060332 (1‑based index). */
seq_t a(idx_t n) {
    /* Static state retained between calls. */
    static bool initialized = false;
    static seq_t *primes = NULL;      /* primes found so far */
    static size_t primes_cap = 0, primes_len = 0;
    static seq_t sum = 0;             /* running sum of last up to 7 composites */
    static uint64_t last[7];          /* circular buffer of the last 7 composites */
    static size_t last_idx = 0;       /* position to overwrite next composite */
    static size_t last_count = 0;     /* number of composites seen so far (max 7) */
    static idx_t cur = 2;             /* current integer to test for composite */

    if (!initialized) {
        /* Initial allocation for primes array. */
        primes = NULL;
        primes_cap = 0;
        primes_len = 0;
        initialized = true;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        /* Find the next composite integer. */
        while (is_prime(cur)) {
            cur++;
        }
        /* cur is composite. */
        /* Update the sliding window and cumulative sum. */
        seq_t comp = (seq_t)cur;              /* cast composite to seq_t for arithmetic */
        if (last_count < 7) {
            last[last_count++] = (uint64_t)comp;
            sum += comp;
        } else {
            sum -= (seq_t)last[last_idx];
            last[last_idx] = (uint64_t)comp;
            sum += comp;
            last_idx = (last_idx + 1) % 7;
        }

        /* If we have a full window, test the sum for primality. */
        if (last_count == 7) {
            if (is_prime((idx_t)sum)) {
                /* Store the prime. */
                grow_seq_array(&primes, &primes_cap, primes_len + 1);
                primes[primes_len++] = sum;
            }
        }

        cur++;  /* advance to next integer for next iteration */
    }

    return primes[n - 1];
}
