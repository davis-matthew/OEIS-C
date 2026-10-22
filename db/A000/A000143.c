/*
 * A000143  Number of ways of writing n as a sum of 8 squares.
 * https://oeis.org/A000143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(-1)^n*Sum_{0<d|n} (-1)^d*d^3, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t d;

    if (n == 0)
        return (seq_t)1;

    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t c = (seq_t)d * (seq_t)d * (seq_t)d;
            if (((n + d) & 1) == 0)
                pos += c;
            else
                neg += c;
            if (e != d) {
                seq_t ce = (seq_t)e * (seq_t)e * (seq_t)e;
                if (((n + e) & 1) == 0)
                    pos += ce;
                else
                    neg += ce;
            }
        }
    }
    return (seq_t)16 * (pos - neg);
}
