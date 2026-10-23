/*
 * A124731  Triangle, row sums = powers of 3, companion to A124730.
 * https://oeis.org/A124731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A124731 – Triangle, row sums = powers of 3
 *
 *  The triangle is produced by iterating a bidiagonal matrix M with
 *  main diagonal 2,1,2,1,… and sub‑diagonal 1,2,1,2,… .
 *  Let row r (r ≥ 0) be the vector v_r of length r+1 given by
 *
 *       v_0  = [1]
 *       v_{r}[0] = 2 * v_{r-1}[0]
 *       v_{r}[i] = d_i * ( v_{r-1}[i] + v_{r-1}[i-1] ),  i ≥ 1
 *
 *  where      d_i = 2  if i even, 1 if i odd.
 *  The global sequence a(n) is the flattening of the triangle
 *  row after row (row 0 has 1 element, row 1 has 2, …).
 *
 *  The routine keeps a cache of all elements up to the largest n
 *  requested so far.  Each call works in amortised O(n) time,
 *  with only O(n) memory for the produced values.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of all values computed so far */
    static seq_t *data = NULL;
    static size_t data_cap = 0;
    static size_t data_len = 0;

    /* previous row needed to generate the next one */
    static seq_t *prev_row = NULL;
    static size_t prev_len = 0;

    /* initialise on first call */
    if (data == NULL) {
        data_cap = 32;
        data = (seq_t *)malloc(data_cap * sizeof(seq_t));
        if (!data) return (seq_t)0;   /* out of memory – term is undefined */

        /* row 0 */
        prev_len = 1;
        prev_row = (seq_t *)malloc(prev_len * sizeof(seq_t));
        if (!prev_row) { free(data); return (seq_t)0; }
        prev_row[0] = (seq_t)1;

        data[0] = (seq_t)1;
        data_len = 1;
    }

    /* if the requested n is already cached, return it */
    if ((idx_t)data_len > n) {
        return data[n];
    }

    /* compute rows until we have a(n) */
    while (data_len <= (size_t)n) {
        size_t r = prev_len;          /* current row index (prev_len = r)   */
        size_t new_len = r + 1;       /* next row length                     */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_row) return (seq_t)0;/* out of memory                       */

        /* first element: 2 * prev_row[0] */
        new_row[0] = (seq_t)2 * prev_row[0];

        /* remaining elements */
        for (size_t i = 1; i < new_len; ++i) {
            seq_t di = (i % 2 == 0) ? (seq_t)2 : (seq_t)1;
            seq_t wi   = (i < prev_len) ? prev_row[i] : (seq_t)0;
            seq_t wim  = prev_row[i - 1];
            new_row[i] = di * (wi + wim);
        }

        /* append the new row to the cache */
        for (size_t i = 0; i < new_len; ++i) {
            if (data_len == data_cap) {
                data_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
                if (!tmp) { free(new_row); return (seq_t)0; }
                data = tmp;
            }
            data[data_len++] = new_row[i];
        }

        /* free the old row and keep the new one for the next step */
        free(prev_row);
        prev_row = new_row;
        prev_len = new_len;
    }

    return data[n];
}
