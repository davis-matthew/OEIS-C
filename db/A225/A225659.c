/*
 * A225659  Primes p where p + sumOfDigits(p) +- 3 is prime.
 * https://oeis.org/A225659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, dumb
 * Formula:   A225659 – primes p with p + sumDigits(p) ± 3 prime
 * Range:     u8 n <= 25, u16 n <= 394, u32 n <= 394, u64 n <= 394,
 *            u128 n <= 394, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a memoized list of terms.  New terms
   are generated on demand by testing successive primes for the
   required property.  It works for 8,16,32,64 or 128‑bit unsigned
   seq_t and for arbitrary‑precision seq_t as supplied by the build.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"

/* Returns the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* static state: memoised terms, current candidate, capacity */
    static seq_t *terms = NULL;
    static idx_t term_count = 0;      /* how many terms have been found */
    static idx_t cap = 0;            /* current capacity of terms array   */
    static seq_t next_candidate = 2; /* next integer to examine as prime */

    /* allocate / grow terms array lazily */
    if (terms == NULL) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) abort();
    }

    /* generate terms until we have at least n of them */
    while (term_count < (idx_t)n) {
        seq_t cand = next_candidate;

        /* test cand for primality */
        if (is_prime_big(cand)) {
            seq_t sod = digit_sum(cand);
            seq_t q1 = cand + sod + 3;
            seq_t q2 = cand + sod - 3;

            /* if p±3 property holds, store cand */
            if (is_prime_big(q1) || is_prime_big(q2)) {
                if ((idx_t)term_count + 1 > cap) {
                    /* double capacity */
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    cap = new_cap;
                }
                terms[term_count++] = cand;
            }
        }

        /* advance to next candidate */
        if (cand == 2)
            next_candidate = 3;   /* after 2 start at 3 */
        else
            next_candidate += 2; /* skip even numbers */
    }

    return terms[(idx_t)n - 1];
}
