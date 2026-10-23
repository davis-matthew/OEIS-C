/*
 * A025354  Numbers that are the sum of 3 distinct nonzero squares in 8 or
 *          more ways.
 * https://oeis.org/A025354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025354: Numbers that are the sum of 3 distinct nonzero squares in at least 8 ways.
   The implementation builds a list of such numbers in ascending order by brute‑force search.
   It memoises the list and enlarges it only when a larger n is requested. 
   For each new limit it recomputes all sums from scratch, which satisfies the time limits
   for n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

/* static state ------------------------------------------------------------ */
static seq_t *terms     = NULL;   /* stored terms in ascending order   */
static size_t terms_len = 0;      /* number of stored terms            */
static size_t terms_cap = 0;      /* capacity of the terms array       */
static idx_t  current_limit = 0;  /* highest number processed          */

/* ------------------------------------------------------------------------- */
/* rebuild the terms array until it contains at least 'needed' elements.
   The computation is performed for a limit that is increased until enough
   terms are found.  Each reconsideration starts from scratch.   */
static void recompute_terms(idx_t needed)
{
    if ((idx_t)terms_len >= needed)
        return;                      /* already enough      */

    idx_t lim = current_limit ? current_limit : (idx_t)50000;   /* start bound */

    while ((idx_t)terms_len < needed) {
        /* --- count representations of all sums up to 'lim' --- */
        size_t *cnt = (size_t *)calloc((size_t)lim + 1, sizeof(size_t));
        if (!cnt)
            return;                                   /* allocation failure */

        idx_t max_i = _floor_isqrt_u64(lim);
        for (idx_t a = 1; a <= max_i; ++a) {
            seq_t a2 = (seq_t)a * a;
            for (idx_t b = a + 1; b <= max_i; ++b) {
                seq_t ab = a2 + (seq_t)b * b;
                if (ab > lim)
                    break;
                for (idx_t c = b + 1; c <= max_i; ++c) {
                    seq_t s = ab + (seq_t)c * c;
                    if (s > lim)
                        break;
                    ++cnt[(size_t)s];
                }
            }
        }

        /* --- rebuild the terms array from the counts --- */
        free(terms);
        terms = NULL;
        terms_len = 0;
        terms_cap = 0;

        for (idx_t s = 1; s <= lim; ++s) {
            if (cnt[(size_t)s] >= 8) {
                if (terms_len == terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) { free(cnt); return; }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)s;
            }
        }

        free(cnt);
        current_limit = lim;

        /* increase the search limit if needed */
        if ((idx_t)terms_len < needed) {
            idx_t new_lim = lim * 2;
            if (new_lim <= lim)            /* overflow protection */
                new_lim = lim + (idx_t)1000000;
            lim = new_lim;
        }
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                       /* undefined for n<1 */

    recompute_terms(n);
    return terms[n - 1];
}
