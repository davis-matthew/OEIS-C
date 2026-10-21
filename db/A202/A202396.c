/*
 * A202396  Triangle T(n,k), read by rows, given by (2, 1/2, 1/2, 0, 0, 0, 0,
 *          0, 0, 0, ...) DELTA (2, -1/2, -1/2, 0, 0, 0, 0, 0, 0, 0, ...)
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A202396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th element of the triangle T(n,k) read by rows.
 *            T(n,k) satisfies T(n,k) = 3*T(n-1,k) + T(n-1,k-1) + T(n-2,k-2) -
 *            T(n-2,k) with T(0,0)=1, T(1,0)=T(1,1)=2 and T(n,k)=0 if k<0 or
 *            n<k
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    seq_t *prev_prev = NULL;   /* row n-2                                                     */
    seq_t *prev     = NULL;   /* row n-1                                                     */
    seq_t *curr     = NULL;   /* current row                                                */
    idx_t r = 0;              /* current row index                                          */

    while (1) {
        size_t row_len = (size_t)(r + 1);

        curr = alloc_seq_array(row_len);
        if (!curr) return 0;        /* out of memory – unexpected for small n                 */

        idx_t start = r * (r + 1) / 2;   /* index of first element in this row                 */

        for (size_t k = 0; k < row_len; ++k) {
            seq_t val;

            if (r == 0 && k == 0) {
                val = 1;
            } else if (r == 1 && (k == 0 || k == 1)) {
                val = 2;
            } else {
                /* recurrence for r >= 2                                         */
                seq_t t1 = (k < (size_t)r)           ? prev[k]   : 0;
                seq_t t2 = (k >= 1)                   ? prev[k-1] : 0;
                seq_t t3 = (k >= 2 && (k-2) < (size_t)(r-1)) ? prev_prev[k-2] : 0;
                seq_t t4 = (prev_prev && k < (size_t)(r-1))        ? prev_prev[k]   : 0;
                val = 3 * t1 + t2 + t3;
                val = val - t4;
            }

            curr[k] = val;

            if (start + k == n) {      /* target found – clean up and return                */
                free(prev_prev);
                free(prev);
                free(curr);
                return val;
            }
        }

        /* shift rows for the next iteration                                     */
        free(prev_prev);
        prev_prev = prev;
        prev = curr;
        curr = NULL;
        ++r;
    }
}
