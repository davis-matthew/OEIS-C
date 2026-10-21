/*
 * A065209  Numbers which need four 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term by brute‑force reverse‑and‑add steps. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_decimal.h"

/* A helper that tests whether k needs exactly four reverse‑and‑add steps
   to reach a palindrome. */
static bool needs_four_steps(seq_t k)
{
    if (is_palindrome(k, 10))
        return false;                  /* steps == 0 → exclude palindromes */

    seq_t curr = k;
    for (int i = 1; i <= 4; ++i) {
        curr = curr + reverse_decimal(curr);
        if (is_palindrome(curr, 10))
            return i == 4;            /* accepted only if reached at step 4 */
    }
    return false;                     /* no palindrome within 4 steps */
}

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static storage for memoised terms. */
    static seq_t *terms     = NULL;   /* array of found terms             */
    static size_t term_count = 0;     /* number of terms stored           */
    static size_t term_cap   = 0;     /* allocated capacity               */
    static seq_t last_checked = 0;    /* last candidate evaluated         */

    /* If the required term is already cached, return it. */
    if (term_count >= n)
        return terms[n - 1];

    /* Initial allocation on first call. */
    if (!terms) {
        term_cap = 64;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        term_count = 0;
        last_checked = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        ++last_checked;                           /* candidate k            */
        if (needs_four_steps(last_checked)) {      /* check the property     */
            if (term_count >= term_cap) {          /* grow array if needed   */
                term_cap = term_cap * 2 + 10;
                terms = (seq_t *)realloc(terms,
                                         term_cap * sizeof(seq_t));
            }
            terms[term_count++] = last_checked;    /* store the new term     */
        }
    }
    return terms[n - 1];
}
