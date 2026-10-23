/*
 * A144911  Triangle read by rows: rad(n) along left edge (n, 1) and core(n)
 *          along right edge (n, n), with (n, k) = core((n - 1, k - 1) + (n -
 *          1, k)) for 1 < k < n when n > 2.
 * https://oeis.org/A144911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) read from triangle defined by * (n,1)=rad(n),
 *            (n,n)=core(n), * (n,k)=core((n-1,k-1)+(n-1,k)) for 1<k<n, n>2 *
 *            initial values: a(1)=1, a(2)=2, a(3)=2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   core_seq, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/rad.h"
#include "../utils/core_seq.h"

/* ---- sequence function a(n) ---- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0 */

    /* Find the row r such that 1 + 2 + ... + r >= n */
    idx_t r = 1;
    idx_t tri = 1;                /* triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Position k (1‑indexed) in row r */
    idx_t prev_tri = tri - r;
    idx_t k = n - prev_tri;       /* 1 <= k <= r */

    seq_t *prev = NULL;
    seq_t *cur  = NULL;

    for (idx_t cur_row = 1; cur_row <= r; ++cur_row) {
        cur = (seq_t *)malloc(cur_row * sizeof(seq_t));
        if (!cur)                /* allocation failed */
            return 0;

        /* Left edge */
        cur[0] = rad(cur_row);

        /* Right edge */
        cur[cur_row - 1] = core_seq((seq_t)cur_row);

        /* Interior cells */
        if (cur_row > 2) {
            for (idx_t j = 1; j < cur_row - 1; ++j) {
                seq_t sum = prev[j - 1] + prev[j];
                cur[j] = core_seq(sum);
            }
        }

        /* If this is the desired row, return the element */
        if (cur_row == r) {
            seq_t result = cur[k - 1];
            free(prev);
            free(cur);
            return result;
        }

        /* Move to next row */
        free(prev);
        prev = cur;
        cur  = NULL;
    }

    /* Should never reach here */
    return 0;
}
