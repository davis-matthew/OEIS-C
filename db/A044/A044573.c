/*
 * A044573  Numbers n such that string 0,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_base8_0_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044573: numbers n such that the base‑8 representation of
 * n contains the consecutive digits 0 followed by 5, but the base‑8
 * representation of n+1 does not.  The function computes the n‑th
 * term by incremental search, memoising all terms it has found.
 *
 *  The implementation works for all supported seq_t types, including
 *  arbitrary‑precision big integer classes.  All arithmetic uses
 *  seq_t directly, avoiding any implicit conversion to narrow types.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_base8_0_5.h"

/* --------------------------------------------------------------------
 * State kept for memoisation
 * -------------------------------------------------------------------- */
static seq_t *terms      = NULL;   /* stored terms */
static size_t terms_cap  = 0;       /* allocated capacity */
static idx_t  term_cnt   = 0;       /* number of terms stored */
static seq_t  last_chk   = 0;       /* last integer examined */

/* --------------------------------------------------------------------
 * Ensure that terms[] can hold at least 'needed' elements.
 * -------------------------------------------------------------------- */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;

#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
    if (terms) {
        for (size_t i = 0; i < term_cnt; ++i) tmp[i] = terms[i];
        delete[] terms;
    }
#else
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
#endif
    terms = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------
 * Return the n‑th term of the sequence (1‑based).  Calls may arrive
 * in any order; the function lazily computes new terms as needed.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of bounds – caller should not use */

    while (term_cnt < n) {
        ++last_chk;  /* next integer to test */

        if (has_base8_0_5(last_chk) && !has_base8_0_5(last_chk + 1)) {
            grow_terms(term_cnt + 1);
            terms[term_cnt++] = last_chk;
        }
    }
    return terms[n - 1];
}
