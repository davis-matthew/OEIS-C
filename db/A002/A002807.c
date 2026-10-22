/*
 * A002807  a(n) = Sum_{k=3..n} (k-1)!*C(n,k)/2.
 * https://oeis.org/A002807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*(n-2)/2 + n*a(n-1) - (n-1)*a(n-2), a(0) = a(1) =
 *            a(2) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p2 = 0; /* a(i-2) */
    seq_t p1 = 0; /* a(i-1) */
    idx_t i;
    if (n <= 2) return (seq_t)0;
    for (i = 3; i <= n; i++) {
        seq_t m1 = (seq_t)(i - 1);
        seq_t m2 = (seq_t)(i - 2);
        seq_t tri;
        seq_t cur;
        if (((i - 1) & 1) == 0)
            tri = (m1 / 2) * m2;
        else
            tri = m1 * (m2 / 2);
        cur = tri + (seq_t)i * p1;
        cur -= m1 * p2;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
