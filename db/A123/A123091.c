/*
 * A123091  Numbers k such that k divides 5^k - 5.
 * https://oeis.org/A123091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 6627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123091: numbers k such that k divides 5^k-5.
   The function builds the sequence by testing each k from 1 upward
   for the property 5^k ≡ 5 (mod k) and stores matching k in a
   static array until the requested index is reached. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

static seq_t *terms = NULL;          /* cached sequence values */
static size_t terms_len = 0;         /* number of values stored */
static size_t terms_cap = 0;         /* capacity of terms array */
static idx_t next_candidate = 1;     /* next integer to test */

/* Grow the terms array to hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – behaviour below 1 is unused */

    while (terms_len < (size_t)n) {
        seq_t k = (seq_t)next_candidate;
        seq_t r = mod_pow((seq_t)5, (uint64_t)next_candidate, k); /* 5^k mod k */

        /* Check if k | (5^k - 5), i.e. 5^k ≡ 5 (mod k). */
        if ((r + k - (seq_t)5) % k == 0) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = k;
        }
        ++next_candidate;
    }
    return terms[n - 1];              /* zero‑based index in the array */
}
