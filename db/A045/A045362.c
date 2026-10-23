/*
 * A045362  Primes congruent to {1, 2, 4, 5} mod 7.
 * https://oeis.org/A045362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 7 ∈ {1,2,4,5}
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p with p mod 7 ∈ {1,2,4,5}. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static cache for the found primes */
static seq_t *desired = NULL;      /* array of qualifying primes */
static size_t desired_cnt = 0;     /* number stored so far */
static size_t desired_cap = 0;     /* current capacity of desired */

/* last candidate checked; persists between calls */
static idx_t last_cand = 2;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0 */

    /* If we already have the value, return it immediately */
    if (desired_cnt >= n) return desired[n - 1];

    /* Allocate initial storage if necessary */
    if (desired_cap == 0) {
        desired_cap = 8;
        desired = (seq_t *)malloc(desired_cap * sizeof(seq_t));
        if (!desired) return 0;           /* allocation failed */
    }

    /* Start searching from the last checked candidate */
    idx_t cand = last_cand;

    while (desired_cnt < n) {
        /* Skip even numbers greater than 2 */
        if (!(cand > 2 && cand % 2 == 0)) {
            if (is_prime(cand)) {
                idx_t r = cand % 7;
                if (r == 1 || r == 2 || r == 4 || r == 5) {
                    /* Grow the array if needed */
                    if (desired_cnt == desired_cap) {
                        desired_cap = desired_cap ? desired_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(desired, desired_cap * sizeof(seq_t));
                        if (!tmp) return 0;   /* allocation failed */
                        desired = tmp;
                    }
                    desired[desired_cnt++] = (seq_t)cand;
                }
            }
        }
        cand++;
    }

    /* Remember the next candidate for subsequent calls */
    last_cand = cand;

    return desired[n - 1];
}
