/*
 * A216472  Smallest k such that k*n^5 + 1 is an n-th power.
 * https://oeis.org/A216472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that k * n^5 + 1 is an
 *            n-th power
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, modexp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/modexp.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t N = (seq_t)n;
    seq_t M = big_pow(N, (seq_t)5);   /* n^5 */

    seq_t m_val = 0;
    for (idx_t m = 2; ; ++m) {
        seq_t base = (seq_t)m % M;
        if (modexp(base, n, M) == 1) {
            m_val = (seq_t)m;
            break;
        }
    }

    seq_t m_pow = big_pow(m_val, (seq_t)n);   /* m^n */
    seq_t k = (m_pow - 1) / M;
    return k;
}
