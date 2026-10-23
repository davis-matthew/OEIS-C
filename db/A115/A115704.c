/*
 * A115704  Triangular numbers whose digit reversal is prime; trailing zeros
 *          are permitted.
 * https://oeis.org/A115704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 14, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115704: Triangular numbers whose digit reversal is prime; trailing zeros allowed.
   Implementation: generate triangular numbers sequentially and keep a cache
   of those whose reverse is prime.  Uses the supplied utilities:
   - rev_decimal(seq_t)   to reverse digits
   - is_prime_big(seq_t)  to test primality of the reversed number. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/rev_decimal.h"
#include "../utils/is_prime_big.h"

/* Helper to append a term to the dynamic cache */
static void append_term(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Return the nth term of A115704 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for 0, return a safe value */

    /* Static state to cache results between calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_tri = 0;  /* last triangular index processed */

    /* Ensure we have at least n qualifying terms cached */
    while (cache_len < (size_t)n) {
        last_tri++;  /* next triangular index */
        seq_t t = (seq_t)last_tri * ((seq_t)last_tri + 1) / 2;  /* t_n */
        seq_t rev = rev_decimal(t);
        if (is_prime_big(rev)) {
            append_term(t, &cache, &cache_len, &cache_cap);
        }
    }

    return cache[(size_t)n - 1];
}
