/*
 * A091534  Generalized Stirling2 array (5,2).
 * https://oeis.org/A091534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the flattened sequence of the generalized Stirling‑2
 *            array for (5,2). Each element a(n,k) is defined by the
 *            recurrence a(n,k) = Σ_{p=0..2} binomial(2,p) *
 *            fallfac(3*(n-1)+k-p, 2-p) * a(n-1,k-p) with base a(1,2)=1 (all
 *            other values zero). The sequence indices run row by row, left to
 *            right,...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See the OEIS entry A091534 for the definition and the first terms. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed sequence values (1‑based) */
    static seq_t *cache = NULL;       /* cache[1..cache_len] */
    static idx_t cache_len = 0;

    /* static storage for the previous row of the array a(n,k) */
    static seq_t *prev_row = NULL;    /* indexed by k, 0..2*current_row */
    static idx_t prev_row_len = 0;    /* size of prev_row array */
    static idx_t current_row = 0;     /* number of the last computed row */

    /* initialise on first call */
    if (cache_len == 0) {
        /* row 1 ----------------------------------------------------------- */
        current_row = 1;
        prev_row_len = 2 * current_row + 1;              /* indices 0..2   */
        prev_row = (seq_t *)malloc(prev_row_len * sizeof(seq_t));
        for (idx_t i = 0; i < prev_row_len; ++i) prev_row[i] = 0;
        prev_row[2] = (seq_t)1;                          /* a(1,2) = 1 */

        cache_len = 1;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        cache[0] = 0;                                      /* unused 0‑index */
        cache[1] = (seq_t)1;                               /* a(1)   */
    }

    /* compute rows until we have the requested index */
    while (cache_len < n) {
        /* --- compute next row ------------------------------------------ */
        idx_t next_row_num = current_row + 1;
        idx_t next_len = 2 * next_row_num + 1;              /* size of new row */
        seq_t *next_row = (seq_t *)malloc(next_len * sizeof(seq_t));

        /* initialise all entries to 0 (not strictly needed) */
        for (idx_t i = 0; i < next_len; ++i) next_row[i] = 0;

        /* recurrence for k = 2 .. 2*next_row_num */
        for (idx_t k = 2; k <= 2 * next_row_num; ++k) {
            seq_t term = (seq_t)0;
            /* falling factorial part: st = 3*(n-1)+k */
            seq_t st = (seq_t)(3 * current_row + k);

            /* p = 0 term : fallfac(st,2) * a(n-1,k) */
            if (k <= 2 * current_row) {
                term += (st * (st - 1)) * prev_row[k];
            }

            /* p = 1 term : 2 * fallfac(st-1,1) * a(n-1,k-1) */
            if ((k - 1) >= 2 && (k - 1) <= 2 * current_row) {
                seq_t val = (seq_t)(st - 1);
                term += (seq_t)2 * val * prev_row[k - 1];
            }

            /* p = 2 term : fallfac(st-2,0) * a(n-1,k-2)  (i.e. just a(n-1,k-2)) */
            if ((k - 2) >= 2 && (k - 2) <= 2 * current_row) {
                term += prev_row[k - 2];
            }

            next_row[k] = term;
        }

        /* --- append new row to the global cache --------------------------- */
        idx_t add_count = 2 * next_row_num - 1;      /* number of k values used */
        cache = (seq_t *)realloc(cache,
                                 (cache_len + add_count + 1) * sizeof(seq_t));

        /* cache is 1‑based, so copy into positions cache_len+1 .. cache_len+add_count */
        idx_t offset = cache_len + 1;
        for (idx_t i = 2, j = 0; i <= 2 * next_row_num; ++i, ++j) {
            cache[offset + j] = next_row[i];
        }
        cache_len += add_count;

        /* free previous row and move to next */
        free(prev_row);
        prev_row = next_row;
        prev_row_len = next_len;
        current_row = next_row_num;
    }

    return cache[n];
}
