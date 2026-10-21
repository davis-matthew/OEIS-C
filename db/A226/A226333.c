/*
 * A226333  Expansion of (E_4(q) - E_4(q^5)) / 240 in powers of q where E_4 is
 *          an Eisenstein series.
 * https://oeis.org/A226333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e) where a(5^e) = 5^(3e) a(p^e) =
 *            (p^(3(e+1)) – 1)/(p^3 – 1) = 1 + p^3 + p^6 + … + p^(3e) (p ≠ 5)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (p == 5) {
                /* a(5^e) = 125^e  (125 = 5^3) */
                seq_t term = 1;
                for (idx_t i = 0; i < e; ++i)
                    term *= (seq_t)125;
                result *= term;
            } else {
                seq_t p3 = (seq_t)p * p * p;           /* p³  */
                seq_t term = 1;
                seq_t pow = 1;
                for (idx_t i = 0; i < e; ++i) {
                    pow *= p3;          /* p^(3(i+1)) */
                    term += pow;
                }
                result *= term;
            }
        }
    }
    /* Remaining prime factor, if any */
    if (tmp > 1) {
        idx_t p = tmp; /* exponent e = 1 */
        if (p == 5) {
            result *= (seq_t)125;          /* 5^(3) */
        } else {
            seq_t p3 = (seq_t)p * p * p;
            result *= (seq_t)(1 + p3);      /* 1 + p³ */
        }
    }
    return result;
}
