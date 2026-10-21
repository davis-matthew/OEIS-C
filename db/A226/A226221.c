/*
 * A226221  Numbers n such that 2^n mod n is not a power of 2.
 * https://oeis.org/A226221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A226221 – numbers n such that 2^n mod n is NOT a power of 2.
   Computed by a simple brute‑force search with memoisation. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_power_of_two.h"

static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t next_candidate = 1;

/* Append a term to the static array, growing it if necessary. */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Return the n-th term of A226221 (n >= 1). */
seq_t a(idx_t n)
{
    while (terms_len < n) {
        idx_t cand = next_candidate++;
        seq_t modval = mod_pow((seq_t)2, (uint64_t)cand, (seq_t)cand);
        if (!is_power_of_two(modval))
            add_term((seq_t)cand);
    }
    return terms[n - 1];
}
