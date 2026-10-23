/*
 * A044668  Numbers n such that string 4,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_40_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th integer such that its base‑9 representation
   contains the substring "40" but the base‑9 representation of a(n)+1 does
   not.  The first few terms are 36, 117, 198, 279, … */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_40_in_base9.h"

/* --------------------------------------------------------------------- */
/* Dynamic array to cache computed terms.                                */
/* --------------------------------------------------------------------- */

#ifdef __cplusplus
static seq_t *terms = nullptr;      /* array of terms */
#else
static seq_t *terms = NULL;         /* array of terms */
#endif
static size_t terms_cap = 0;        /* allocated capacity */
static size_t terms_len = 0;        /* number of stored terms */
static idx_t next_candidate = 0;    /* next number to test */

/* Ensure at least 'need' capacity. */
static void ensure_capacity(size_t need)
{
    if (need <= terms_cap)
        return;

    size_t newcap = terms_cap == 0 ? 16 : terms_cap * 2;
    while (newcap < need)
        newcap *= 2;

#ifdef __cplusplus
    seq_t *newptr = new seq_t[newcap];
    for (size_t i = 0; i < terms_len; ++i)
        newptr[i] = terms[i];
    delete[] terms;
    terms = newptr;
#else
    seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!newptr)
        exit(1);
    terms = newptr;
#endif
    terms_cap = newcap;
}

/* Append a term to the cache. */
static void append_term(seq_t val)
{
    ensure_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Public function required by the framework.                            */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)  /* undefined, but return 0 for safety */
        return 0;

    /* Generate terms until we have at least 'n' of them. */
    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        bool cand_has = has_40_in_base9(cand);
        bool succ_has = has_40_in_base9(cand + (seq_t)1);
        if (cand_has && !succ_has)
            append_term(cand);
        ++next_candidate;
    }
    return terms[n - 1];
}
