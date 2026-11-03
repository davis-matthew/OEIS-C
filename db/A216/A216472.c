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
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t modexp(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t N = (seq_t)n;
    seq_t M = ipow(N, (seq_t)5);   /* n^5 */

    seq_t m_val = 0;
    for (idx_t m = 2; ; ++m) {
        seq_t base = (seq_t)m % M;
        if (modexp(base, n, M) == 1) {
            m_val = (seq_t)m;
            break;
        }
    }

    seq_t m_pow = ipow(m_val, (seq_t)n);   /* m^n */
    seq_t k = (m_pow - 1) / M;
    return k;
}
