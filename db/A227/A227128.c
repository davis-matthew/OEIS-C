/*
 * A227128  The twisted Euler phi-function for the non-principal Dirichlet
 *          character mod 3.
 * https://oeis.org/A227128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n*Π_{p|n} (1-chi(p)/p) for the non‑principal Dirichlet
 *            character mod 3. Multiplicative definition: a(3^e) = 3^e a(p^e)
 *            = p^(e-1)*(p-1) if p ≡ 1 (mod 3) a(p^e) = p^(e-1)*(p+1) if p ≡ 2
 *            (mod 3)
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t tmp = n;
    /* trial factorisation – n ≤ 10000 gives ~√n steps */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t factor;
            if (p == 3) {
                /* a(3^e) = 3^e */
                factor = 1;
                for (idx_t i = 0; i < exp; ++i) factor *= p;
            } else if (p % 3 == 1) {
                /* a(p^e) = p^(e-1)*(p-1)  (p ≡ 1 mod 3) */
                seq_t power = 1;
                for (idx_t i = 0; i < exp - 1; ++i) power *= p;
                factor = power * (p - 1);
            } else { /* p % 3 == 2 */
                /* a(p^e) = p^(e-1)*(p+1)  (p ≡ 2 mod 3) */
                seq_t power = 1;
                for (idx_t i = 0; i < exp - 1; ++i) power *= p;
                factor = power * (p + 1);
            }
            result *= factor;
        }
    }
    /* remaining prime factor (if any) has exponent 1 */
    if (tmp > 1) {
        seq_t factor;
        if (tmp == 3) {
            factor = 3;
        } else if (tmp % 3 == 1) {
            factor = tmp - 1;
        } else { /* tmp % 3 == 2 */
            factor = tmp + 1;
        }
        result *= factor;
    }
    return result;
}
