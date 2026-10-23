/*
 * A145501  Dirichlet g.f.: (1+1/4^s+4/16^s)*zeta(s)^3.
 * https://oeis.org/A145501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative with a(2) = 3, a(2^e) = 3*(e-2)*(e-1)+7 for e >
 *            1, a(p^e) = (e+1)*(e+2)/2 for p > 2
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)    /* should not happen */
        return 0;

    seq_t   result = 1;
    idx_t   m      = n;

    /* factorise m by trial division */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t term;
            if (p == 2) {
                if (e == 1) {
                    term = 3;
                } else {
                    term = (seq_t)3 * (e - 2) * (e - 1) + 7;
                }
            } else {
                term = ((seq_t)(e + 1) * (e + 2)) / 2;
            }
            result *= term;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        /* m is a prime > 2, exponent e = 1 → term = (1+1)*(1+2)/2 = 3 */
        result *= 3;
    }

    return result;
}
