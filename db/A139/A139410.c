/*
 * A139410  Numbers n such that n = 4^d_1 + 4^d_2 + ... + 4^d_k where
 *          d_1d_2...d_k is the decimal expansion of n.
 * https://oeis.org/A139410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, bref
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139410: numbers n such that
 * n = 4^d1 + 4^d2 + … + 4^dk   where d1…dk is the decimal
 * expansion of n (OEIS definition).  The function below
 * generates the terms on demand, caching all previously found
 * values so that calls are O(1) after the initial search.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initialise digit‑power table once */
    static seq_t pow4[10];
    static int pow4_done = 0;
    if (!pow4_done) {
        pow4[0] = (seq_t)1;
        for (int d = 1; d < 10; ++d)
            pow4[d] = pow4[d - 1] * (seq_t)4;
        pow4_done = 1;
    }

    /* cache of terms found so far */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static idx_t last_check = 1;  /* next number to evaluate */

    /* helper to grow the terms array */
    if (terms_cnt == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* extend the search until we have at least n terms */
    while (terms_cnt < n) {
        seq_t sum = (seq_t)0;
        idx_t tmp = last_check;
        while (tmp) {
            int digit = tmp % 10;
            sum += pow4[digit];
            tmp /= 10;
            /* small optimisation: abort if sum already exceeds candidate */
            if (sum > (seq_t)last_check) {
                /* sum too large, no chance for equality */
                break;
            }
        }
        if (sum == (seq_t)last_check) {
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp2 = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp2) abort();
                terms = tmp2;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = last_check;
        }
        ++last_check;
    }

    return terms[(size_t)(n - 1)];
}
