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
 * Validated: standard, pass, 80 terms through n = 9085 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching integers and testing whether each integer is
   divisible by the sum of its distinct prime factors. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the sum of the distinct prime factors of an unsigned integer n. */
static seq_t distinct_prime_sum(idx_t n)
{
    idx_t temp = n;
    seq_t sum = 0;

    /* handle factor 2 */
    if ((temp & 1) == 0) {
        sum += 2;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* handle odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            sum += p;
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime > 1 */
    if (temp > 1) sum += temp;

    return sum;
}

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
                seq_t *tmp = mem_resize(seq_t, memo, new_cap);
                memo = tmp;
                mem_cap = new_cap;
            }
            memo[mem_len++] = (seq_t)cand;
        }
    }

    return memo[n - 1];   /* offset 1: a(1) is the first term. */
}
