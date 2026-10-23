/*
 * A033802  Product t2(q^d); d | 43, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of non‑negative integer pairs (m,k) such that *
 *            m(m+1)/2 + 43*k(k+1)/2 = n. * Equivalent to counting odd
 *            positive solutions to x² + 43y² = 8n + 44, * with x = 2m+1, y =
 *            2k+1
 * Range:     u8 n <= 9, u16 n <= 4490, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    idx_t k = (idx_t)0;

    for (;;)
    {
        /* T_k = k(k+1)/2 as seq_t */
        seq_t tk = (seq_t)k * (k + (idx_t)1) / (seq_t)2;

        /* term = 43 * T_k */
        seq_t term = (seq_t)43 * tk;

        if (term > (seq_t)n)
            break;

        /* remaining value t = n - term */
        seq_t t = (seq_t)n - term;

        /* Check whether t is a triangular number:
         * 8*t + 1 must be a perfect square. */
        seq_t d = (seq_t)8 * t + (seq_t)1;
        seq_t r = isqrt_seq(d);
        if (r * r == d)
            count = count + (seq_t)1;

        k = k + (idx_t)1;
    }

    return count;
}
