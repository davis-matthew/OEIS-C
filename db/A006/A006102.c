/*
 * A006102  Gaussian binomial coefficient [ n,4 ] for q=3.
 * https://oeis.org/A006102
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n,4]_3 via the q-Pascal recurrence [m,k] = [m-1,k-1] +
 *            3^k*[m-1,k], [m,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t c[5];
    static const int qk[5] = {1, 3, 9, 27, 81};
    int j;
    idx_t m;
    c[0] = (seq_t)1;
    for (j = 1; j <= 4; j++) c[j] = (seq_t)0;
    for (m = 1; m <= n; m++) {
        for (j = 4; j >= 1; j--) {
            c[j] = c[j - 1] + c[j] * (seq_t)qk[j];
        }
    }
    return c[4];
}
