/*
 * A059219  Variation of Boustrophedon transform applied to sequence
 *          1,0,0,0,...: fill an array by diagonals in alternating directions
 *          - 'up' and 'down'. The first element of each diagonal after the
 *          first is 0. When 'going up', add to the previous element the
 *          elements of the row the new element is in. When '...
 * https://oeis.org/A059219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_table_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059219
 *  Formula: a(n) is the final element of the n‑th diagonal in a
 *  variation of the Boustrophedon transform applied to the
 *  sequence 1,0,0,0,...  (see the description in the problem text).
 *  The first element of each diagonal after the first is 0.  For an
 *  odd diagonal (going up) each new element is computed as
 *
 *      value = previous + sum_of_row_of_new_element
 *
 *  For an even diagonal (going down) the sum is taken over the
 *  column of the new element.  The final element of the diagonal is
 *  a(n).  The algorithm below does this directly, keeping running
 *  sums of each row and each column, so no intermediate factorial
 *  or binomial divisions are required.  The implementation works
 *  both for ordinary fixed‑width integer types and for big‑int
 *  types (e.g. unsigned __int128 or an arbitrary‑precision class).
 */

#include <stdlib.h>
#include "../utils/ensure_table_size.h"

/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static state used for memoisation */
    static seq_t *row_sum = NULL; /* sum of each row so far   */
    static seq_t *col_sum = NULL; /* sum of each column so far */
    static seq_t *cache   = NULL; /* cached a(k) values       */
    static size_t row_cap = 0, col_cap = 0, cache_cap = 0;
    static idx_t  computed = 0;   /* highest diagonal already computed */

    /* initialise on first call */
    if (row_sum == NULL) {
        row_cap   = 1;
        col_cap   = 1;
        cache_cap = 1;

        row_sum = (seq_t*)malloc(row_cap * sizeof(seq_t));
        col_sum = (seq_t*)malloc(col_cap * sizeof(seq_t));
        cache   = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!row_sum || !col_sum || !cache) abort();

        row_sum[0] = (seq_t)1;      /* a(0) = 1 in row 0 */
        col_sum[0] = (seq_t)1;      /* column 0, same value */
        cache[0]   = (seq_t)1;      /* a(0) */
        computed = 0;
    }

    /* compute all diagonals up to n if needed */
    for (idx_t d = computed + 1; d <= n; ++d) {
        /* ensure we have room for indices up to d */
        ensure_table_size(&row_sum, &row_cap, d);
        ensure_table_size(&col_sum, &col_cap, d);
        ensure_table_size(&cache,   &cache_cap, d);

        seq_t prev = (seq_t)0;  /* first element of a new diagonal is 0 (except d==0 which is handled above) */

        if (d % 2 == 1) {
            /* odd diagonal – direction "up": from (d,0) to (0,d) */
            for (idx_t k = 1; k <= d; ++k) {
                idx_t r = d - k;  /* row index   */
                idx_t c =   k;    /* column index*/
                seq_t row_val = row_sum[r];
                seq_t val = prev + row_val;
                row_sum[r] += val;
                col_sum[c] += val;
                prev = val;
            }
            cache[d] = prev;   /* last element is a(d) */
        } else {
            /* even diagonal – direction "down": from (0,d) to (d,0) */
            for (idx_t k = 1; k <= d; ++k) {
                idx_t r =   k;    /* row index   */
                idx_t c = d - k;  /* column index*/
                seq_t col_val = col_sum[c];
                seq_t val = prev + col_val;
                row_sum[r] += val;
                col_sum[c] += val;
                prev = val;
            }
            cache[d] = prev;
        }

        computed = d;
    }

    return cache[n];
}
