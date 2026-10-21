/*
 * A079710  Numbers m such that the squarefree kernel of m is larger than the
 *          squarefree kernel of m+1.
 * https://oeis.org/A079710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by searching integers m in increasing order and
 *            collecting those for which the squarefree kernel rad(m) is
 *            larger than rad(m+1). The function keeps a cache so that calls
 *            in any order are fast. rad(m) is provided by ../utils/rad.h
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rad.h"

/* Cache of already found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Highest integer m that has already been examined. */
static idx_t current_checked = 1;

/* Expand the cache to contain at least `target` terms. */
static void ensure_terms(idx_t target)
{
    /* Treat target as a size_t; n is tiny (≤10000) so no overflow. */
    while (terms_len < (size_t)target) {
        idx_t m = current_checked + 1;                     /* candidate m   */
        seq_t rad_m   = rad(m);                            /* rad(m)        */
        seq_t rad_next = rad(m + 1);                       /* rad(m+1)      */

        if (rad_m > rad_next) {                            /* record term   */
            if (terms_cap == 0) {
                terms_cap = 8;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            } else if (terms_len + 1 > terms_cap) {
                terms_cap <<= 1;                           /* double cap   */
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)m;
        }
        current_checked = m;                               /* advance state */
    }
}

/* Main access function for A079710. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                            /* undefined for n<1 */
    ensure_terms(n);                                         /* build cache up to n */
    return terms[n - 1];                                     /* zero‑based index   */
}
