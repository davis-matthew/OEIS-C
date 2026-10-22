/*
 * A000090  Expansion of e.g.f. exp((-x^3)/3)/(1-x).
 * https://oeis.org/A000090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - (n-1)*(n-2)*a(n-3) + (n-1)*(n-2)*(n-3)*a(n-4),
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t w0, w1, w2, w3, next, c;
    idx_t m;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* w0 = a(m-4), w1 = a(m-3), w2 = a(m-2), w3 = a(m-1) */
    w0 = (seq_t)1;
    w1 = (seq_t)1;
    w2 = (seq_t)2;
    w3 = (seq_t)4;
    for (m = 4; m <= n; m++) {
        c = (seq_t)(m - 1) * (seq_t)(m - 2);
        next = (seq_t)m * w3;
        next += c * (seq_t)(m - 3) * w0;
        next -= c * w1;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = next;
    }
    return w3;
}
