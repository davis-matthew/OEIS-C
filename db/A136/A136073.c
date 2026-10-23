/*
 * A136073  Primes of the form 9*p + 8 with p prime.
 * https://oeis.org/A136073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 250, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th prime of the form 9*p + 8, where p itself is prime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

/* Cached sequence values. */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Last integer p that was examined. */
static uint64_t last_checked = 1; /* start just before the first prime */

/* Ensure the terms array has space for at least 'needed' elements. */
static void grow_terms(size_t needed)
{
    if (terms_len >= needed) return;
    if (terms_cap == 0) terms_cap = 4;
    while (terms_cap < needed) terms_cap <<= 1;
    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
}

/* Generate terms up to index n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (terms_len >= (size_t)n) return; /* already have enough */

    uint64_t cand;
    for (cand = last_checked + 1; terms_len < (size_t)n; ++cand)
    {
        /* Skip even numbers > 2. */
        if (cand > 2 && !(cand & 1U)) continue;

        if (!is_prime(cand)) continue;        /* composite */

        /* p = cand is prime. */
        seq_t q = (seq_t)9 * (seq_t)cand + (seq_t)8;   /* 9*p + 8 */

        if (is_prime_big(q))
        {
            grow_terms(terms_len + 1);
            terms[terms_len++] = q;
        }
    }
    /* last_checked becomes the largest p examined. */
    last_checked = cand - 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    generate_up_to(n);               /* fill cache if required */
    return terms[n - 1];              /* 1‑based index */
}
