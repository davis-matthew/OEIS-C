/*
 * A153652  Triangle T(n, k, j) = T(n-1, k, j) + T(n-1, k-1, j) + (2*j +
 *          1)*prime(j)*T(n-2, k-1, j) with T(2, k, j) = prime(j) and j = 7,
 *          read by rows.
 * https://oeis.org/A153652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 40, u64 n <= 144, u128 n <= 543,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) with j = 7:
   T(1,1) = 2;
   T(2,k) = prime(7) = 17;
   T(3,1) = T(3,3) = 2,   T(3,2) = 2*prime(7)^2 - 4;
   T(n,1) = T(n,n) = 2          (n ≥ 1);
   For n ≥ 4 and 1 < k < n:
        T(n,k) = T(n-1,k) + T(n-1,k-1) + (2*7+1)*prime(7)*T(n-2,k-1).
   a(n) is obtained by reading the rows of this triangle left‑to‑right,
   top‑to‑bottom (row 1, row 2, …).  Example: a(1)=2, a(2)=17, a(3)=17,
   a(4)=2, a(5)=574, … */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: the sequence values and row start indices */
    static seq_t *data = NULL;
    static size_t data_cap = 0;
    static size_t data_len = 0;
    static size_t *row_start = NULL;   /* 1‑based index: row_start[i] is where row i starts in data */
    static size_t row_cap = 0;
    static size_t rows = 0;            /* number of rows computed so far */

    /* constants for j = 7 */
    const uint64_t j = 7ULL;
    const seq_t prime_j = (seq_t)17;                /* prime(7) */
    const seq_t coeff   = (seq_t)((2 * j + 1) * prime_j); /* (2*j+1)*prime(j) = 255 */

    /* compute enough rows to cover the requested term a(n) */
    while (data_len < (size_t)n) {
        size_t next_row = rows + 1;          /* 1‑based row number */

        /* ensure row_start array is large enough */
        if (row_cap <= next_row) {
            size_t new_cap = row_cap ? row_cap * 2 : 8;
            while (new_cap <= next_row) new_cap *= 2;
            size_t *tmp = (size_t *)realloc(row_start, new_cap * sizeof(size_t));
            if (!tmp) return 0;          /* allocation failed – return 0 as error marker */
            row_start = tmp;
            row_cap = new_cap;
        }

        /* ensure data array can hold the new row */
        if (data_cap < data_len + next_row) {
            size_t new_cap = data_cap ? data_cap * 2 : 8;
            while (new_cap < data_len + next_row) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            data = tmp;
            data_cap = new_cap;
        }

        /* store the start index of this row */
        row_start[next_row] = data_len;

        /* generate the row */
        if (next_row == 1) {                    /* row 1 */
            data[data_len++] = (seq_t)2;
        } else if (next_row == 2) {             /* row 2 */
            data[data_len++] = prime_j;
            data[data_len++] = prime_j;
        } else if (next_row == 3) {             /* row 3 */
            data[data_len++] = (seq_t)2;                            /* k = 1 */
            data[data_len++] = 2 * prime_j * prime_j - (seq_t)4;   /* k = 2 */
            data[data_len++] = (seq_t)2;                            /* k = 3 */
        } else {                                /* rows ≥ 4 */
            for (size_t k = 1; k <= next_row; ++k) {
                if (k == 1 || k == next_row) {
                    data[data_len++] = (seq_t)2;
                } else {
                    seq_t t1 = data[row_start[next_row - 1] + (k - 1)];
                    seq_t t2 = data[row_start[next_row - 1] + (k - 2)];
                    seq_t t3 = data[row_start[next_row - 2] + (k - 2)];
                    data[data_len++] = t1 + t2 + coeff * t3;
                }
            }
        }
        ++rows;
    }
    return data[n - 1];          /* a(1) is stored at data[0] */
}
