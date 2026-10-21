/*
 * A055801  Triangle T read by rows: T(i,0)=T(i,i)=1, T(i,j) =
 *          Sum_{k=1..floor(n/2)} T(i-2k, j-2k+1) for 1<=j<=i-1, where T(m,n)
 *          := 0 if m<0 or n<0.
 * https://oeis.org/A055801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 728, u32 n <= 2533, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from triangle T:
   T(i,0) = T(i,i) = 1.
   For 1 <= j <= i-1,
        T(i,j) = sum_{k=1..floor(i/2)} T(i-2k, j-2k+1)   (T(m,n)=0 if m<0 or n<0).
   a(n) = T(i,j) where
        i = floor((sqrt(8*n+1)-1)/2)
        j = n - i*(i+1)/2
   (The mapping follows the row‑by‑row enumeration of the triangle.
   row i starts at index i*(i+1)/2.) */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t **table = NULL;   /* rows of the triangle */
    static idx_t max_i_computed = 0;  /* highest row already built */

    /* locate row i and column j for index n */
    idx_t low = 0, high = 1;
    while ((seq_t)high * (high + 1) / 2 <= n) high <<= 1;   /* exponential search */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t i = low;
    idx_t j = n - i * (i + 1) / 2;

    /* initialise the table on first call */
    if (table == NULL) {
        table = (seq_t **)malloc(sizeof(seq_t *));
        if (table == NULL) return 0;
        table[0] = (seq_t *)malloc(sizeof(seq_t));
        if (table[0] == NULL) return 0;
        table[0][0] = 1;
        max_i_computed = 0;
    }

    /* extend the table up to row i */
    if (i > max_i_computed) {
        /* enlarge the array of row pointers */
        seq_t **new_table = (seq_t **)realloc(table, (i + 1) * sizeof(seq_t *));
        if (new_table == NULL) return 0;
        table = new_table;

        /* compute each new row from max_i_computed+1 .. i */
        for (idx_t r = max_i_computed + 1; r <= i; ++r) {
            seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
            if (row == NULL) return 0;
            for (idx_t c = 0; c <= r; ++c) {
                if (c == 0 || c == r) {
                    row[c] = 1;
                } else {
                    seq_t sum = 0;
                    idx_t maxk = r / 2;
                    for (idx_t k = 1; k <= maxk; ++k) {
                        /* add only when the source index is non‑negative */
                        if (c >= 2 * k - 1)
                            sum += table[r - 2 * k][c - 2 * k + 1];
                    }
                    row[c] = sum;
                }
            }
            table[r] = row;
        }
        max_i_computed = i;
    }

    return table[i][j];
}
