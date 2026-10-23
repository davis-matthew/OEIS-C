/*
 * A106088  Primes p such that 7*p + 6 and 6*p + 7 are primes.
 * https://oeis.org/A106088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derivation: a(n) is the nth prime p such that 7p+6 and 6p+7 are also primes. */
/* The function generates such primes on demand and memoises them. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"    /* provides: bool is_prime(uint64_t n) */

seq_t a(idx_t n)
{
    /* Handle offset and invalid indices. */
    if (n < 1) return 0;

    /* Static storage for memoisation. */
    static seq_t *terms = NULL;      /* array of found primes */
    static size_t terms_cap = 0;     /* allocated capacity */
    static size_t terms_size = 0;    /* current number of terms */
    static idx_t last_checked = 1;   /* last candidate inspected */

    /* Allocate initial storage if needed. */
    if (terms == NULL) {
        terms_cap = 1024;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) return 0; /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (terms_size < (size_t)n) {
        /* Next candidate to test. */
        last_checked++;
        uint64_t cand = (uint64_t)last_checked;

        /* Check if cand is prime. */
        if (!is_prime(cand)) continue;

        /* Check the two derived numbers.  Use uint64_t to avoid overflow. */
        uint64_t a1 = 7ULL * cand + 6ULL;
        uint64_t a2 = 6ULL * cand + 7ULL;
        if (!is_prime(a1) || !is_prime(a2)) continue;

        /* Found a valid prime; store it. */
        if (terms_size == terms_cap) {
            terms_cap <<= 1; /* double capacity */
            seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            terms = tmp;
        }
        terms[terms_size++] = (seq_t)cand;
    }

    return terms[n - 1];
}
