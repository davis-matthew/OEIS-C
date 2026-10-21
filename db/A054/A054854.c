/*
 * A054854  Number of ways to tile a 4 X n region with 1 X 1 and 2 X 2 tiles.
 * https://oeis.org/A054854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-2*a(n-3); a(0) = 1, a(1) = 1, a(2) = 5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms    = NULL;   /* array of stored terms */
static idx_t  terms_len = 0;     /* number of terms currently stored (terms_len == i+1 for last computed index i) */
static size_t terms_cap = 0;     /* allocated capacity */

static void ensure_size(idx_t n)
{
    if ((idx_t)terms_len > n) return;           /* already have it */

    size_t needed = (size_t)(n + 1);            /* elements required up to index n */
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? (terms_cap << 1) : 8;
        while (new_cap < needed) new_cap <<= 1;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }

    /* compute new terms starting from the current length */
    for (idx_t i = terms_len; i <= n; ++i) {
        if (i == 0)           terms[0] = 1;
        else if (i == 1)      terms[1] = 1;
        else if (i == 2)      terms[2] = 5;
        else
            terms[i] =
              (seq_t)2 * terms[i-1]             /* 2*a(n-1) */
            + (seq_t)3 * terms[i-2]             /* +3*a(n-2) */
            - (seq_t)2 * terms[i-3];            /* -2*a(n-3) */
    }
    terms_len = n + 1;    /* new highest index is n */
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return terms[n];
}
