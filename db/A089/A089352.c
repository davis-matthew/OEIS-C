/*
 * A089352  Numbers that are divisible by the sum of their distinct prime
 *          factors (A008472).
 * https://oeis.org/A089352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 7650, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   distinct_prime_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching integers and testing whether each integer is
   divisible by the sum of its distinct prime factors. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/distinct_prime_sum.h"

/* Array of previously found terms. */
static seq_t *memo = NULL;      /* cached terms */
static size_t mem_len = 0;      /* number of cached terms */
static size_t mem_cap = 0;      /* capacity of memo array */
static idx_t last_candidate = 1;/* last integer tested */

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Search until at least n terms have been generated. */
    while (mem_len < n) {
        last_candidate++;
        idx_t cand = last_candidate;

        seq_t s = distinct_prime_sum(cand);
        if (cand % (idx_t)s == 0) {
            /* Grow cache if needed. */
            if (mem_len == mem_cap) {
                size_t new_cap = mem_cap ? mem_cap * 2 : 128;
                seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
                memo = tmp;
                mem_cap = new_cap;
            }
            memo[mem_len++] = (seq_t)cand;
        }
    }

    return memo[n - 1];   /* offset 1: a(1) is the first term. */
}
