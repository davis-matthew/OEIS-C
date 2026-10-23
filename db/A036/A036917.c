/*
 * A036917  Expansion of g.f. (4/Pi^2)*EllipticK(4*x^(1/2))^2.
 * https://oeis.org/A036917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (C(2*(n-k), n-k) * C(2*k, k))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t sum = 0;
    seq_t b1 = 1;                 /* C(0,0) */
    seq_t b2 = binom(2 * n, n);   /* C(2*n, n) for k=0 */

    for (idx_t i = 0; i <= n; ++i) {
        /* term = (b1 * b2)^2 */
        seq_t prod  = b1 * b2;
        seq_t term  = prod * prod;
        sum += term;

        if (i == n) break;        /* no more values needed */

        /* Update b1 to C(2*(i+1), i+1) */
        idx_t k1   = i + 1;
        seq_t top1 = (seq_t)2           * (seq_t)k1
                   * (seq_t)(2 * k1 - 1);
        seq_t bot1 = (seq_t)k1 * (seq_t)k1;
        b1 = b1 * top1 / bot1;

        /* Update b2 to C(2*(n-k1), n-k1) */
        idx_t m_prev = n - i;           /* current m = n - i   */
        seq_t num2 = (seq_t)m_prev * (seq_t)m_prev;
        seq_t den2 = (seq_t)(2 * m_prev) * (seq_t)(2 * m_prev - 1);
        b2 = b2 * num2 / den2;
    }

    return sum;
}
