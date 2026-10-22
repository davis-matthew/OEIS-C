/*
 * A002493  Number of ways to arrange n non-attacking kings on an n X n board,
 *          with 2 sides identified to form a cylinder, with 1 in each row and
 *          column.
 * https://oeis.org/A002493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) - 2*Sum_{k>=0, n-1-2k>=1} b(n-1-2k) for n>=3, where
 *            b = A002464: b(n) = (n+1)*b(n-1) - (n-2)*b(n-2) - (n-5)*b(n-3) +
 *            (n-3)*b(n-4), b(0)=1, b(1)=1, b(2)=0, b(3)=0, b(4)=2; a(1)=1,
 *            a(2)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n <= 4) return 0;

    /* rolling window: w0=b(m-4), w1=b(m-3), w2=b(m-2), w3=b(m-1) */
    seq_t w0 = 1, w1 = 1, w2 = 0, w3 = 0;
    /* s[p] = sum of b(j) for 1 <= j < current m with j % 2 == p */
    seq_t s[2];
    s[0] = 0;          /* b(2) */
    s[1] = 1;          /* b(1) + b(3) */
    seq_t bm = 0;
    for (idx_t m = 4; m <= n; m++) {
        if (m == 4) {
            bm = 2;
        } else {
            seq_t pos = (seq_t)(m + 1) * w3 + (seq_t)(m - 3) * w0;
            seq_t neg = (seq_t)(m - 2) * w2 + (seq_t)(m - 5) * w1;
            bm = pos - neg;
        }
        if (m < n) s[m & 1] += bm;
        w0 = w1; w1 = w2; w2 = w3; w3 = bm;
    }
    return bm - (seq_t)2 * s[(n - 1) & 1];
}
