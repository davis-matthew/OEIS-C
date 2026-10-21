/*
 * A205524  Numbers n such that gcd(n, sigma(n)) is not equal to sigma(n) mod
 *          n.
 * https://oeis.org/A205524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite integer that is NOT a perfect number *
 *            and satisfies gcd(n, sigma(n)) != sigma(n) mod n. * Here
 *            sigma(n) is the sum of all positive divisors of n (including n).
 *            * The sequence is the complement of the list of n where gcd(n,
 *            sigma(n)) * equals sigma(n) mod n, but excludin...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A205524_H
#define A205524_H

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Sum of divisors sigma(n) for a 64‑bit integer n.  */
/* Result fits in seq_t (or expands to arbitrary precision). */
static seq_t sigma_u64(uint64_t n)
{
    seq_t sum = 1;               /* 1 always divides n */
    if (n <= 1) return sum;     /* sigma(1) = 1 */

    for (uint64_t i = 2; i <= n / i; ++i) {
        if ((n % i) == 0) {
            sum += (seq_t)i;
            uint64_t d = n / i;
            if (d != i) sum += (seq_t)d;
        }
    }
    sum += (seq_t)n;             /* add n itself */
    return sum;
}

/* Dynamic array of terms found so far. */
static seq_t *terms = NULL;
static size_t term_len = 0;   /* number of terms stored */
static size_t term_cap = 0;   /* allocated capacity */
static uint64_t next_cand = 2;/* next integer to test */

/* Ensure there is room for another term. */
static void grow_if_needed(void)
{
    if (term_len == term_cap) {
        size_t new_cap = term_cap ? (term_cap << 1) : 32;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    while (term_len < (size_t)n) {
        uint64_t cand = next_cand++;

        seq_t sigma = sigma_u64(cand);

        /* Skip perfect numbers (sigma == 2 * cand). */
        if (sigma == ((seq_t)cand << 1))
            continue;

        seq_t g = gcd((seq_t)cand, sigma);
        seq_t mod = sigma % (seq_t)cand;

        if (g != mod) {
            grow_if_needed();
            terms[term_len++] = (seq_t)cand;
        }
    }
    return terms[(size_t)n - 1];
}

#endif /* A205524_H */
