/*
 * A054612  a(n) = Sum_{d|n} phi(d)*5^(n/d).
 * https://oeis.org/A054612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * 5^(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t phi_uint64(idx_t m)
{
    if (m <= 1) return (seq_t)m;
    seq_t res = (seq_t)m;
    idx_t m_copy = m;
    for (idx_t p = 2; p * p <= m_copy; ++p) {
        if (m_copy % p == 0) {
            while (m_copy % p == 0)
                m_copy /= p;
            res = res / (seq_t)p * ((seq_t)p - 1);
        }
    }
    if (m_copy > 1)              /* last prime factor */
        res = res / (seq_t)m_copy * ((seq_t)m_copy - 1);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Pre‑compute powers of 5 up to n */
    seq_t *pow5 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!pow5) return (seq_t)0;
    pow5[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow5[i] = pow5[i - 1] * (seq_t)5;

    seq_t sum = (seq_t)0;

    /* Sum over all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            seq_t phi_d = phi_uint64(d);
            sum += phi_d * pow5[q];
            if (d != q) {
                seq_t phi_q = phi_uint64(q);
                sum += phi_q * pow5[d];
            }
        }
    }

    free(pow5);
    return sum;
}
