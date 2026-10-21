/*
 * A067240  If n = Product_{i} p_i^e_i, a(n) = Sum_{i} (p_i - 1)*p_i^(e_i -
 *          1).
 * https://oeis.org/A067240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e ∥ n} (p-1) * p^(e-1) (OEIS A067240)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External linkage – not static */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1) return (seq_t)0;

    seq_t result = (seq_t)0;
    idx_t m = n;

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* term = (p-1) * p^(e-1) */
            seq_t term = (seq_t)p - 1;   /* (p-1) */
            seq_t p_pow = 1;
            for (idx_t i = 1; i < e; ++i) {
                p_pow *= (seq_t)p;      /* p^(e-1) */
            }
            term *= p_pow;
            result += term;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        result += (seq_t)m - 1;        /* p-1, e = 1 -> p^(e-1) = 1 */
    }

    return result;
}
