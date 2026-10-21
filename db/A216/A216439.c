/*
 * A216439  Smallest k such that k*n^3 + 1 is an n-th power.
 * https://oeis.org/A216439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k*n^3 + 1 is an n‑th power
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n = 1 is a special case */
    if (n == (idx_t)1) return (seq_t)1;

    seq_t n_big = (seq_t)n;
    seq_t n3 = n_big * n_big * n_big;   /* n^3 */

    idx_t m = 2;                        /* minimal m > 1 */
    for (;; ++m)
    {
        seq_t m_big = (seq_t)m;
        seq_t residue = big_pow(m_big, n_big) % n3;
        if (residue == (seq_t)1) break;
    }

    seq_t m_pow = big_pow((seq_t)m, n_big);
    seq_t k = (m_pow - (seq_t)1) / n3;
    return k;
}
