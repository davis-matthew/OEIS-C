/*
 * A084354  Primes whose product of nonzero digits + 1 is prime.
 * https://oeis.org/A084354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th prime p such that product of the non‑zero
 *            decimal digits of p, plus 1, is also prime
 * Range:     u8 n <= 20, u16 n <= 2630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, prod_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/prod_nonzero_digits.h"

/* static storage */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t next_candidate = 2;

/* enlarge the terms array if needed */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_terms = new seq_t[new_cap];
    for (size_t i = 0; i < terms_len; ++i)
        new_terms[i] = terms[i];
    delete[] terms;
    terms = new_terms;
#else
    seq_t *new_terms = realloc(terms, new_cap * sizeof(seq_t));
    terms = new_terms;
#endif
    terms_cap = new_cap;
}

/* append a new term to the stored sequence */
static void append_term(uint64_t val)
{
    ensure_terms_capacity(terms_len + 1);
    terms[terms_len++] = (seq_t)val;
}

/* return the nth term (offset 1) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    while (terms_len < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            seq_t prod = prod_nonzero_digits((seq_t)next_candidate);
            idx_t check = (idx_t)(prod + (seq_t)1);
            if (is_prime(check))
                append_term((uint64_t)next_candidate);
        }
        ++next_candidate;
    }
    return terms[(size_t)n - 1];   /* zero‑based index */
}
