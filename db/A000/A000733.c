/*
 * A000733  Boustrophedon transform of partition numbers 1, 1, 1, 2, 3, 5, 7,
 *          ...
 * https://oeis.org/A000733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) in the boustrophedon array with T(k,0) = b(k),
 *            T(k,j) = T(k,j-1) + T(k-1,k-j), where b(0) = 1 and b(k) = p(k-1)
 *            for k >= 1, p = partition numbers (A000041)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   release, zeroed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/release.h"
#include "../utils/zeroed.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* partition numbers p(0) .. p(n-1) */
    seq_t *p = zeroed(n);
    p[0] = 1;
    for (idx_t m = 1; m < n; m++)
        for (idx_t i = m; i < n; i++)
            p[i] += p[i - m];

    seq_t *prev_row = zeroed(n + 1);
    seq_t *curr_row = zeroed(n + 1);
    prev_row[0] = 1;

    for (idx_t row = 1; row <= n; row++) {
        if (row % 2 == 1) {
            curr_row[0] = p[row - 1];
            for (idx_t k = 1; k <= row; k++)
                curr_row[k] = curr_row[k - 1] + prev_row[k - 1];
        } else {
            curr_row[row] = p[row - 1];
            for (idx_t k = row; k-- > 0; )
                curr_row[k] = curr_row[k + 1] + prev_row[k];
        }
        seq_t *t = prev_row;
        prev_row = curr_row;
        curr_row = t;
    }

    seq_t result = (n % 2 == 0) ? prev_row[0] : prev_row[n];

    release(prev_row, n + 1);
    release(curr_row, n + 1);
    release(p, n);
    return result;
}
