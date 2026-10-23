/*
 * A140335  Smallest number greater than a(n-1) that, when adding the
 *          reciprocals of all the terms up to it, the sum is <= 3.
 * https://oeis.org/A140335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A140335:
 * a(1) = 1.
 * For n ≥ 2,
 *   a(n) = smallest integer m > a(n‑1) such that
 *          Σ_{i=1}^{n} 1/a(i) ≤ 3.
 * The sequence stops at n = 13 because the sum then equals 3 exactly.
 *
 * Implementation uses an incremental algorithm keeping the sum as a
 * reduced rational a/b.  All arithmetic is performed on seq_t, the
 * integer type supplied by the build.  No floating‑point operations
 * are used.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* 1‑based array: terms[1] .. terms[terms_len] */
    static idx_t  terms_cap = 0;
    static idx_t  terms_len = 0;
    static seq_t  sum_num = 0;       /* numerator of the current sum */
    static seq_t  sum_den = 1;       /* denominator of the current sum */

    /* Allocate the array on first use. */
    if (terms == NULL) {
        terms_cap = 32;   /* initial capacity */
        terms = (seq_t *)malloc((terms_cap + 1) * sizeof(seq_t));
    }

    /* Generate terms up to the requested index. */
    while (terms_len < n) {
        /* First term: a(1) = 1. */
        if (terms_len == 0) {
            seq_t v = 1;
            if (terms_len + 1 > terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, (terms_cap + 1) * sizeof(seq_t));
            }
            terms_len++;
            terms[terms_len] = v;
            sum_num = v;   /* sum == 1/1 */
            sum_den = 1;
            continue;
        }

        /* Compute 3 - sum = (3*sum_den - sum_num) / sum_den. */
        seq_t diff_num = 3 * sum_den - sum_num;
        if (diff_num == 0) {
            /* The sum has reached 3; no further terms exist. */
            return 0;
        }

        /* The smallest m that satisfies 1/m ≤ diff = diff_num / sum_den
           is ceil(sum_den / diff_num). */
        seq_t target = (sum_den + diff_num - 1) / diff_num; /* ceil division */

        seq_t prev = terms[terms_len];
        seq_t candidate = prev + 1;
        if (candidate < target) candidate = target;        /* a(n) = max(prev+1, target) */

        /* Store the new term. */
        if (terms_len + 1 > terms_cap) {
            terms_cap *= 2;
            terms = (seq_t *)realloc(terms, (terms_cap + 1) * sizeof(seq_t));
        }
        terms_len++;
        terms[terms_len] = candidate;

        /* Update the sum:  sum = sum + 1/candidate */
        seq_t new_num = sum_num * candidate + sum_den;
        seq_t new_den = sum_den * candidate;
        seq_t g = gcd(new_num, new_den);                 /* reduce the fraction */
        new_num /= g;
        new_den /= g;
        sum_num = new_num;
        sum_den = new_den;
    }

    return terms[n];
}
