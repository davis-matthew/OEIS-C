/*
 * A084201  Primes p such that the sum of the digits is prime and the sum of
 *          the digits of 2p is also prime.
 * https://oeis.org/A084201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 685, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084201: primes p with prime digit sum and prime digit sum of 2p */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime.h"

static seq_t *terms   = NULL;   /* array of sequence values */
static size_t term_len = 0;      /* number of stored terms   */
static size_t term_cap = 0;      /* capacity of terms array */

static idx_t current_candidate = 2;   /* next number to test */

/* Grow the terms array when needed and store a new term. */
static void append_term(idx_t p)
{
    if (term_len == term_cap)
    {
        term_cap = term_cap ? term_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_len++] = (seq_t)p;
}

/* Ensure that at least n terms are stored. */
static void ensure_n_terms(idx_t n)
{
    if (term_len >= (size_t)n)
        return;                     /* already have enough terms */

    /* If the terms array hasn't been allocated yet, do so. */
    if (!terms)
    {
        term_cap = 8;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    }

    while ((idx_t)term_len < n)
    {
        /* Test current_candidate for primality. */
        if (is_prime(current_candidate))
        {
            seq_t sum1 = digit_sum((seq_t)current_candidate);
            if (is_prime_small((idx_t)sum1))
            {
                seq_t doubled = ((seq_t)current_candidate) * (seq_t)2;
                seq_t sum2   = digit_sum(doubled);
                if (is_prime_small((idx_t)sum2))
                    append_term(current_candidate);
            }
        }

        /* Advance to the next candidate.  Skip even numbers after 2. */
        if (current_candidate == 2)
            current_candidate = 3;
        else
            current_candidate += 2;
    }
}

/* Public interface: nth element of A084201 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety, unused for n ≥ 1 */

    ensure_n_terms(n);
    return terms[n - 1];
}
