/*
 * A059594  Convolution triangle based on A008619 (positive integers
 *          repeated).
 * https://oeis.org/A059594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059594
 *
 *  This function returns the n‑th term of the sequence A059594.
 *  The sequence is obtained by flattening the lower‑triangular array
 *  a(n,m) read row by row.  The array is defined by:
 *
 *    a(0,0)                               = 1
 *    a(n,0)                               = floor((n+2)/2)   (A008619)
 *    a(n,m) = a(n-1,m) + a(n-1,m-1) + a(n-2,m) - a(n-3,m)   for n ≥ m ≥ 1
 *    a(n,m) = 0   if n < m
 *
 *  The algorithm keeps a running cache of all rows that have been
 *  computed and of the flattened array of all terms.  When a new
 *  term is requested the cache is extended until the required index
 *  is present.  The implementation is fully generic in seq_t
 *  (unsigned integer or arbitrary‑precision class) and works as C11
 *  or C++17 code.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached state */
    static size_t last_row = (size_t)-1;              /* index of last computed row */
    static seq_t *row[3] = {NULL, NULL, NULL};       /* [0]=row n-1, [1]=row n-2, [2]=row n-3 */
    static size_t row_len[3] = {0, 0, 0};             /* lengths of the three stored rows */

    static seq_t *seq = NULL;          /* flattened array of all terms */
    static size_t seq_len = 0;          /* number of terms stored */
    static size_t seq_cap = 0;          /* capacity of seq */

    /* Generate rows until we have at least n+1 terms */
    while (seq_len <= n) {
        size_t r = last_row + 1;               /* next row to compute */

        /* Allocate memory for the new row */
        seq_t *cur = (seq_t *)calloc(r + 1, sizeof(seq_t));
        if (!cur) abort();                     /* out of memory */

        /* Compute the row values */
        if (r == 0) {
            cur[0] = (seq_t)1;
        } else {
            for (size_t m = 0; m <= r; ++m) {
                seq_t val = 0;
                if (m < row_len[0])          val += row[0][m];
                if (m > 0 && (m - 1) < row_len[0]) val += row[0][m - 1];
                if (m < row_len[1])          val += row[1][m];
                if (m < row_len[2])          val -= row[2][m];
                cur[m] = val;
            }
        }

        /* Append the new row to the flattened sequence */
        if (seq_len + (r + 1) > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : r + 1;
            while (new_cap < seq_len + (r + 1)) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }
        for (size_t i = 0; i <= r; ++i) seq[seq_len + i] = cur[i];
        seq_len += r + 1;

        /* Rotate the row buffer: discard oldest, shift, insert new */
        free(row[2]);                 /* discard row n-3 */
        row[2] = row[1];
        row_len[2] = row_len[1];
        row[1] = row[0];
        row_len[1] = row_len[0];
        row[0] = cur;
        row_len[0] = r + 1;

        last_row = r;
    }

    return seq[n];
}
