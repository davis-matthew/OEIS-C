/*
 * A158034  Integers n for which f = (4^n - 2^n + 8n^2 - 2) / (2n * (2n + 1))
 *          is an integer.
 * https://oeis.org/A158034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 96, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158034: a(n) is the nth integer m such that
   (4^m - 2^m + 8m^2 - 2) / (2m(2m+1)) is an integer. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static idx_t cur_candidate = 1;
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a value to the terms cache, expanding storage if necessary. */
static void cache_append(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_ptr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();            /* allocation failure */
        terms = new_ptr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    /* Generate terms up to the requested index n. */
    while (terms_len < n) {
        /* Check whether cur_candidate satisfies the condition. */
        seq_t seq_n = (seq_t)cur_candidate;

        /* Denominator D = 2n(2n+1). */
        seq_t two_n = 2 * seq_n;
        seq_t D = two_n * (two_n + 1);

        /* Compute N mod D: 4^n - 2^n + 8n^2 - 2, modulo D. */
        seq_t pow4 = mod_pow(4, cur_candidate, D);
        seq_t pow2 = mod_pow(2, cur_candidate, D);

        seq_t num = pow4;
        num -= pow2;
        num += 8 * seq_n * seq_n;
        num -= 2;

        /* Normalise modulo D (allow negative intermediate values). */
        num = (num % D + D) % D;

        if (num == 0)
            cache_append(seq_n);          /* store the valid term */

        cur_candidate++;                 /* move to the next integer */
    }

    /* Return the (n‑th) cached term (1‑based indexing). */
    return terms[n - 1];
}
