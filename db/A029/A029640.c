/*
 * A029640  Even numbers in the (1,2)-Pascal triangle A029635.
 * https://oeis.org/A029640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth even entry of the (1,2)-Pascal triangle,
 *            enumerated row‑by‑row. For each row the diagonal element (value
 *            2) comes first, then the remaining even elements of that row are
 *            listed in increasing column order
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* previous row, initially the zero‑row [1] */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;
    size_t len_prev = 1;

    idx_t cumulative = 0;               /* number of even entries seen so far */

    for (;;) {                          /* iterate over rows */
        size_t len_new = len_prev + 1;
        seq_t *newrow = (seq_t *)malloc(sizeof(seq_t) * len_new);

        /* build the new row */
        newrow[0] = (seq_t)1;
        for (size_t j = 1; j < len_prev; ++j)
            newrow[j] = prev[j-1] + prev[j];
        newrow[len_new-1] = (seq_t)2;    /* the diagonal element of this row */

        /* count how many even numbers appear in this row */
        idx_t evens = 1;                 /* the diagonal element 2 */
        for (size_t j = 1; j + 1 < len_new; ++j) /* exclude first & last cell */
            if ((newrow[j] & (seq_t)1) == 0)
                ++evens;

        /* if the desired index lies in this row, compute and return it */
        if (cumulative + evens >= n) {
            idx_t offset = n - cumulative - 1;    /* 0‑based within this row */
            seq_t ans;
            if (offset == 0) {                    /* diagonal element */
                ans = (seq_t)2;
            } else {
                idx_t needed = offset;            /* count of off‑diagonal evens to skip */
                for (size_t j = 1; j + 1 < len_new; ++j) {
                    if ((newrow[j] & (seq_t)1) == 0) {
                        if (--needed == 0) {
                            ans = newrow[j];
                            break;
                        }
                    }
                }
            }
            /* clean up */
            free(prev);
            free(newrow);
            return ans;
        }

        /* move to next row */
        cumulative += evens;
        free(prev);
        prev = newrow;
        len_prev = len_new;
    }
}
