/*
 * A207823  Triangle of coefficients of Chebyshev's S(n,x+4) polynomials
 *          (exponents of x in increasing order).
 * https://oeis.org/A207823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is generated from the triangle T(n,k) = 4*T(n‑1,k) +
 *            T(n‑1,k‑1) – T(n‑2,k) (n>1) with T(0,0) = 1 and T(‑1,*) = 0. The
 *            flattened sequence a(n) is the row‑by‑row concatenation of the
 *            triangle entries. The function builds the required rows on
 *            demand, memoises all terms up to the largest request...
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised data */
    static seq_t *flat = NULL;          /* flattened sequence            */
    static idx_t flat_len = 0;          /* number of values stored       */
    static seq_t *prev = NULL;          /* previous row (n‑1)            */
    static seq_t *prevprev = NULL;      /* row before previous (n‑2)     */
    static idx_t prev_len = 0;          /* length of prev                */
    static idx_t prevprev_len = 0;      /* length of prevprev            */
    static idx_t row = 0;               /* index of last generated row   */

    /* Initialisation: row 0 */
    if (flat_len == 0) {
        prev        = (seq_t *)calloc(1, sizeof(seq_t));
        if (!prev) exit(1);
        prev[0]     = (seq_t)1;
        prev_len    = 1;
        /* prevprev is a dummy array of length 0   */
        prevprev    = NULL;
        prevprev_len= 0;

        /* store row 0 in the flat array */
        flat       = (seq_t *)calloc(1, sizeof(seq_t));
        if (!flat) exit(1);
        flat[0]    = prev[0];
        flat_len   = 1;
        row        = 0;
    }

    /* Generate further rows until we have enough terms */
    while (flat_len <= n) {
        idx_t next_row = row + 1;      /* index of the row to generate */
        idx_t new_len  = next_row + 1; /* number of elements in the row */

        seq_t *new_row = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_row) exit(1);

        for (idx_t k = 0; k < new_len; ++k) {
            seq_t a_val = 0, b_val = 0, c_val = 0;

            if (k < prev_len)      a_val = (seq_t)4 * prev[k];
            if (k > 0 && k-1 < prev_len) b_val = prev[k-1];
            if (k < prevprev_len)  c_val = prevprev[k];

            new_row[k] = a_val + b_val - c_val;   /* unsigned arithmetic */
        }

        /* Append the new row to the flattened array */
        seq_t *tmp = (seq_t *)realloc(flat,
                                      (flat_len + new_len) * sizeof(seq_t));
        if (!tmp) exit(1);
        flat = tmp;
        for (idx_t i = 0; i < new_len; ++i)
            flat[flat_len + i] = new_row[i];

        flat_len += new_len;

        /* Shift the row pointers for the next iteration */
        if (prevprev) free(prevprev);
        prevprev      = prev;
        prevprev_len  = prev_len;

        prev          = new_row;
        prev_len      = new_len;

        row = next_row;
    }

    return flat[n];
}
