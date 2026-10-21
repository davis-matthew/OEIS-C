/*
 * A070805  a(1) = 2; a(n) = largest prime not exceeding the sum of all
 *          previous terms.
 * https://oeis.org/A070805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3; for n>=3, a(n) = largest prime not exceeding the
 *            sum of all previous terms
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 17, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *terms = NULL;      /* dynamically allocated array of terms */
static idx_t  max_n  = 0;        /* highest index that has been computed */
static seq_t  sum    = 0;        /* sum of terms[1..max_n] */

/* Find the largest prime <= s */
static seq_t prev_prime_leq(seq_t s)
{
    if (s < 2) return 2;
    seq_t cand = s;
    if (cand > 2 && (cand & 1) == 0) cand -= 1;   /* make it odd if >2 */
    while (true) {
        if (is_prime_big(cand)) return cand;
        if (cand == 2) return 2;                  /* fallback */
        cand -= 2;                                 /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* Undefined; return 0 for safety */

    /* Allocate an array large enough for the current request */
    if (terms == NULL) {
        terms = seq_alloc_new_array((size_t)n + 1); /* index 0 unused */
        if (terms == NULL) return 0;              /* Allocation failed */
        for (size_t i = 0; i <= (size_t)n; ++i) terms[i] = 0;
        sum   = 0;
        max_n = 0;
    }

    /* Grow the array if necessary */
    if (n > max_n) {
        seq_t *new_terms = seq_alloc_new_array((size_t)n + 1);
        if (new_terms == NULL) return 0;         /* Allocation failed */

        for (idx_t i = 1; i <= max_n; ++i) new_terms[i] = terms[i];
        seq_free_array(terms);
        terms = new_terms;

        /* Compute new terms */
        if (max_n == 0) {                /* initialise a(1) */
            terms[1] = 2;
            sum = 2;
            max_n = 1;
        }
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prime;
            if (i == 2) {
                prime = 3;                        /* special case */
            } else {
                prime = prev_prime_leq(sum);
            }
            terms[i] = prime;
            sum += prime;
        }
        max_n = n;
    }
    return terms[n];
}
