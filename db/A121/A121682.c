/*
 * A121682  Triangle read by rows: T(i,j) = (T(i-1,j) + i)*i.
 * https://oeis.org/A121682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(i,j) = (T(i-1,j)+i)*i  (i>=2, 1<=j<=i),
   T(1,1)=1.  The sequence a(n) is read row‑wise from this
   triangle, with offset 1 (a(1) is the first term). */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, but not needed */

    /* Special case for the first term */
    if (n == 1) return (seq_t)1;

    /* Current row indices */
    idx_t i = 1;                  /* current row number, starting at 1 */
    seq_t *row = NULL;            /* pointer to current row values */
    size_t row_len = 0;           /* length of current row */
    idx_t count = 0;              /* total number of terms generated so far */

    /* Initialize with the first row */
    row_len = 1;
    row = (seq_t*)malloc(row_len * sizeof(seq_t));
    if (!row) return 0;           /* allocation failure: shouldn't happen */
    row[0] = (seq_t)1;
    count = 1;                    /* one term generated */

    /* Generate rows until we cover the desired index n */
    while (count < n) {
        ++i;                      /* move to next row i */
        size_t new_len = (size_t)i;

        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_row) { free(row); return 0; }

        /* Compute first (i-1) entries: T(i,j) = (T(i-1,j)+i)*i */
        seq_t inc = (seq_t)i;
        for (size_t j = 0; j < new_len - 1; ++j) {
            new_row[j] = (row[j] + inc) * inc;
        }
        /* Diagonal entry T(i,i) = i*i */
        new_row[new_len - 1] = inc * inc;

        free(row);                /* discard previous row */
        row = new_row;
        row_len = new_len;
        count += (idx_t)row_len;  /* update term counter */

        if (count >= n) {         /* term found in this row */
            idx_t offset = n - (count - (idx_t)row_len); /* 1‑based within row */
            seq_t result = row[offset - 1];
            free(row);
            return result;
        }
    }

    free(row);
    return 0;                       /* should never reach here */
}
