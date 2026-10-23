/*
 * A140776  A number n is included if (p + n/p) is prime, where p is the
 *          largest prime that divides n.
 * https://oeis.org/A140776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 9256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A140776: n is included iff p + n/p is prime,
 * where p is the largest prime divisor of n.
 * The function returns the nth term (1‑based index).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_divisor.h" /* uint64_t largest_prime_divisor(uint64_t m); */
#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t n); */

static seq_t *terms = NULL;   /* cached sequence values */
static size_t terms_cap = 0;  /* capacity of terms array */
static size_t terms_cnt = 0;  /* number of stored terms */
static idx_t current = 2;     /* next integer to test */

/* Append a value to the static cache, growing the storage if necessary. */
static void append_term(seq_t v)
{
    if (terms_cnt == terms_cap) {
        terms_cap = terms_cap ? terms_cap << 1 : 16;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_cnt++] = v;
}

/* Main sequence function. Generates terms lazily until the requested
   index is available.  Caches results for subsequent calls in any order. */
seq_t a(idx_t n)
{
    if (!terms) {
        /* initialise cache */
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_cnt = 0;
        current = 2;
    }

    /* produce terms until we have at least n terms */
    while (terms_cnt < n) {
        uint64_t p = largest_prime_divisor((uint64_t)current);
        seq_t val = (seq_t)p + (seq_t)(current / p);
        if (is_prime_big(val)) {
            append_term((seq_t)current);
        }
        current++;
    }

    return terms[n - 1];
}
