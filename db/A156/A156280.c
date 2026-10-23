/*
 * A156280  A higher order recursion triangle sequence: m=4;l=4;e(n,k,m)=(l*k
 *          + m - 1)e(n - 1, k, m) + (m*n - l*k + 1 - m)e(n - 1, k - 1, m).
 * https://oeis.org/A156280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened higher‑order recursion
 *            triangle with m=4 and l=4. The triangle is defined by e(0,0)=1
 *            and for n≥1, 0<k<n=> e(n,k) = (4*k+3)*e(n-1,k) +
 *            (4*n-4*k+1)*e(n-1,k-1). The first and last entries of each row
 *            are 1. The sequence a(n) is obtained by listing the rows...
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment.
   No additional includes are allowed. */

seq_t a(idx_t n)
{
    /* Maximum sizes needed for n up to 10000:
       - row 141 has length 142, so we need arrays of at least 200.
       - the flattened sequence needs at most 10153 elements,
         so a buffer of 20000 is safe. */
    enum { MAX_ROW   = 200,  /* > 142 */
           MAX_FLAT  = 20000 };

    static seq_t prev[MAX_ROW];
    static seq_t cur [MAX_ROW];
    static seq_t flat[MAX_FLAT];
    static idx_t flat_used = 0;   /* number of elements already stored */
    static idx_t last_row  = 0;   /* index of the last computed row */
    static bool   init      = false;

    /* One‑time initialization */
    if (!init) {
        prev[0] = (seq_t)1;          /* row 0 */
        flat[0] = prev[0];
        flat_used = 1;
        last_row = 0;
        init = true;
    }

    /* Generate rows until we have stored up to index n */
    while (flat_used <= n) {
        idx_t next_row = last_row + 1;          /* index of new row */
        idx_t new_len = next_row + 1;           /* length of new row */

        /* Build new row */
        cur[0] = (seq_t)1;
        for (idx_t k = 1; k < new_len - 1; ++k) {
            seq_t term1 = ((seq_t)(4 * k + 3)) * prev[k];
            seq_t term2 = ((seq_t)(4 * next_row - 4 * k + 1)) * prev[k - 1];
            cur[k] = term1 + term2;
        }
        cur[new_len - 1] = (seq_t)1;

        /* Append newly produced elements to the flattened array */
        for (idx_t j = 0; j < new_len; ++j) {
            flat[flat_used++] = cur[j];
        }

        /* Prepare for the next iteration */
        for (idx_t j = 0; j < new_len; ++j) {
            prev[j] = cur[j];
        }
        last_row = next_row;
    }

    return flat[n];
}
