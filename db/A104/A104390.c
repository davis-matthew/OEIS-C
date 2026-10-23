/*
 * A104390  2-Smith numbers.
 * https://oeis.org/A104390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 813, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_uint64, is_two_smith, sp_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑Smith numbers: a(n) is the n‑th integer >1 whose sum of digit sums of its
 * prime factors (counted with multiplicity) equals twice the digit sum of the
 * integer itself.  The function determines the terms by brute‑force search
 * using trial‑division factorisation.  No analytic formula is known.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum_uint64.h"
#include "../utils/is_two_smith.h"
#include "../utils/sp_of.h"

/*--------- helper: trial‑division factorisation (64‑bit) -------------*/

/*--------- public function: the sequence a(n) -----------------------*/
seq_t a(idx_t n)
{
    /* Lazy static cache of found 2‑Smith numbers */
    static seq_t *terms    = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    /* candidate counter; starts at the first possible 2‑Smith number */
    static uint64_t next_candidate = 32;

    /* grow cache if needed */
    if (terms == NULL) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }
    while (terms_len < (size_t)n) {
        /* advance to next candidate having the property */
        while (!is_two_smith(next_candidate)) ++next_candidate;
        /* store the found term */
        terms[terms_len] = (seq_t)next_candidate;
        ++terms_len;
        ++next_candidate;
        /* enlarge cache when full */
        if (terms_len == terms_cap) {
            terms_cap <<= 1;
            size_t new_bytes = terms_cap * sizeof(seq_t);
            seq_t *tmp = (seq_t *)realloc(terms, new_bytes);
            if (!tmp) exit(EXIT_FAILURE);
            terms = tmp;
        }
    }

    return terms[n - 1];
}
