/*
 * A023206  Numbers m such that m and 2*m + 7 both prime.
 * https://oeis.org/A023206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 521, u32 n <= 559, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as all primes m such that 2*m+7 is also prime.
   For n≥1, a(n) is the n‑th such prime.  The function below builds the
   sequence on demand and caches the results for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *terms = NULL;          /* array of found terms */
    static idx_t term_count = 0;         /* number of terms stored */
    static idx_t capacity = 0;           /* current allocation size   */
    static seq_t next_candidate = 5;     /* next m to test (prime ≥5) */

    /* Helper to grow the terms array when needed */
    if (term_count == 0) { /* first call – initialise cache with 2,3 */
        capacity = 128;
        terms = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!terms) abort();               /* allocation fail */
        terms[0] = (seq_t)2;
        terms[1] = (seq_t)3;
        term_count = 2;
        next_candidate = 5; /* start checking from 5 */
    }

    /* Generate additional terms until we have at least n of them */
    while (term_count < n) {
        /* Ensure capacity for the new term */
        if (term_count >= capacity) {
            capacity *= 2;
            seq_t *tmp = (seq_t*)realloc(terms, sizeof(seq_t) * capacity);
            if (!tmp) abort();
            terms = tmp;
        }

        /* Test candidate m */
        if (is_prime_big(next_candidate)) {
            seq_t s = 2 * next_candidate + 7; /* 2*m+7 */
            if (is_prime_big(s)) {
                terms[term_count++] = next_candidate;
            }
        }

        /* Advance to the next odd integer */
        next_candidate += 2;
    }

    return terms[n - 1];
}
