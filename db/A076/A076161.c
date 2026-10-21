/*
 * A076161  Numbers n such that n + sum of squares of digits of n (A258881) is
 *          a prime.
 * https://oeis.org/A076161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 6874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A076161: Numbers n such that n + sum of squares of digits of n is prime.
 * Formula: a(n) = the n‑th integer m ≥ 1 with m + Σ(di²) prime.
 * This implementation enumerates terms on demand and caches them.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_u64.h"

/* cached sequence */
static seq_t *terms  = NULL;
static size_t term_len  = 0; /* number of stored terms */
static size_t term_cap  = 0; /* capacity of terms array */
static idx_t  last_seen = 0; /* last number tested */

/* test if m + sum_of_squares_of_digits(m) is prime */
static bool good(idx_t m)
{
    idx_t t   = m;
    idx_t sum = 0;
    while (t != 0) {
        int d = t % 10;
        sum += (idx_t)(d * d);
        t   /= 10;
    }

    /* use 64‑bit test if seq_t fits into 8 bytes, otherwise use arbitrary‑precision */
    if (sizeof(seq_t) <= 8) {
        uint64_t cand = (uint64_t)m + (uint64_t)sum;
        return is_prime_u64(cand);
    } else {
        seq_t cand = (seq_t)m + (seq_t)sum;
        return is_prime_big(cand);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset < 1 */

    /* if we already have the term, return it */
    if ((idx_t)term_len >= n)
        return terms[n - 1];

    /* allocate or grow cache as needed */
    if (term_len == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        term_cap = new_cap;
    }

    /* generate terms until we reach the desired index */
    while ((idx_t)term_len < n) {
        ++last_seen;
        if (good(last_seen)) {
            terms[term_len++] = (seq_t)last_seen;
            /* ensure capacity for next insertion */
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms = tmp;
                term_cap = new_cap;
            }
        }
    }
    return terms[n - 1];
}
