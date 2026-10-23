/*
 * A045371  Primes congruent to {1, 2, 4} mod 5.
 * https://oeis.org/A045371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes congruent to 1, 2, or 4 modulo 5. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/grow_idx.h"

/* The build supplies the types seq_t and idx_t. */

static idx_t *all_primes      = NULL;     /* All primes discovered (for trial divisions) */
static size_t all_cap        = 0;
static size_t all_cnt        = 0;

static seq_t *sel_primes     = NULL;     /* Primes with residue 1,2,4 mod 5 */
static size_t sel_cap        = 0;
static size_t sel_cnt        = 0;

static idx_t next_candidate  = 2;         /* Number next to test for primality */

/* Dynamically grow a seq_t array */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        *arr = new_arr;
        *cap = new_cap;
    }
}

/* Test whether n is prime using trial division by all_primes */
static bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;

    for (size_t i = 0; i < all_cnt; ++i) {
        idx_t p = all_primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is undefined for n=0 */

    /* Ensure we have at least n selected primes */
    while (sel_cnt < n) {
        idx_t cand = next_candidate;

        /* Test the candidate for primality */
        if (is_prime_small(cand)) {
            /* Store the prime in the full prime list */
            grow_idx(&all_primes, &all_cap, all_cnt + 1);
            all_primes[all_cnt++] = cand;

            /* If the prime satisfies the residue condition, store it */
            idx_t r = cand % 5;
            if (r == 1 || r == 2 || r == 4) {
                grow_seq(&sel_primes, &sel_cap, sel_cnt + 1);
                sel_primes[sel_cnt++] = (seq_t)cand;
            }
        }

        /* Advance to next candidate: after 2, skip evens */
        if (cand == 2) next_candidate = 3;
        else           next_candidate += 2;
    }

    return sel_primes[n - 1];
}
