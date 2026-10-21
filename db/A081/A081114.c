/*
 * A081114  Triangle read by rows: T(n,k) = n*T(n-1,k) + n - k starting at
 *          T(n,n)=0.
 * https://oeis.org/A081114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read row‑by‑row from the triangle T(n,k) = n * T(n-1,k)
 *            + n - k (0 ≤ k < n, T(n,n) = 0), with T(0,0) = 0. Thus a(n) =
 *            T(r,c) where (r,c) is the triangular index of n. The function
 *            builds the triangle lazily; it stores each row only once. All
 *            arithmetic is done in the type seq_t, which may...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for the list of a(n) values and the last row */
    static seq_t *values      = NULL;   /* flattened triangle */
    static size_t values_len  = 0;     /* number of values stored   */
    static size_t values_cap  = 0;     /* allocated capacity       */

    static seq_t *prev_row    = NULL;   /* values of the previous row T(r,*) */
    static size_t prev_len    = 0;      /* length of prev_row (r+1)        */

    /* ---------- initialise on first call ---------- */
    if (values == NULL) {
        /* row 0 : T(0,0) = 0 */
        values_cap = 1;
        values_len = 1;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        values[0] = (seq_t)0;

        /* previous row is the same as the first row */
        prev_len = 1;
        prev_row = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev_row[0] = (seq_t)0;
    }

    /* ---------- build rows until we have a(n) ---------- */
    while (values_len <= (size_t)n) {
        /* current row index = prev_len - 1 + 1 = prev_len */
        size_t cur_row    = prev_len;          /* r + 1          */
        size_t cur_len    = cur_row + 1;       /* number of elements in this row */

        /* allocate memory for the new row */
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) return (seq_t)0; /* if malloc fails, return 0 (unlikely) */

        /* compute the new row */
        for (size_t k = 0; k < cur_len - 1; ++k) {
            cur[k] = (seq_t)cur_row * prev_row[k] + (seq_t)cur_row - (seq_t)k;
        }
        cur[cur_len - 1] = (seq_t)0;   /* T(n,n) = 0 */

        /* ensure the values array has enough capacity */
        if (values_len + cur_len > values_cap) {
            size_t new_cap = values_cap > 0 ? values_cap : 1;
            while (new_cap < values_len + cur_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* realloc failure */
            values = tmp;
            values_cap = new_cap;
        }

        /* append the new row to the flattened array */
        for (size_t i = 0; i < cur_len; ++i) {
            values[values_len++] = cur[i];
        }

        /* free the old row and replace with the new one */
        free(prev_row);
        prev_row = cur;
        prev_len = cur_len;
    }

    /* ---------- return the requested term ---------- */
    return values[(size_t)n];
}
