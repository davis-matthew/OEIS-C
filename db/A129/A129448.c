/*
 * A129448  Expansion of q * psi(-q) * chi(q^3)^2 * psi(-q^9) in powers of q
 *          where psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A129448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129448: Multiplicative definition with prime‑power rules
    a(1) = 1
    a(p^e) = 
        (-1)^e                     if p = 2
        0                           if p = 3
        e+1                         if p ≡ 1 (mod 12)
        (-1)^e * (e+1)              if p ≡ 5 (mod 12)
        (1+(-1)^e)/2 (0 or 1)       if p ≡ 7 or 11 (mod 12)
    a(n) is multiplicative.
*/
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t factor;

        if (p == 2) {
            factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)-1;
        } else if (p == 3) {
            return (seq_t)0;
        } else {
            uint64_t mod12 = p % 12;
            if (mod12 == 1) {
                factor = (seq_t)(e + 1);
            } else if (mod12 == 5) {
                seq_t sign = (e % 2 == 0) ? (seq_t)1 : (seq_t)-1;
                factor = sign * (seq_t)(e + 1);
            } else { /* mod12 == 7 or 11 */
                factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }

        if (factor == (seq_t)0) return (seq_t)0;
        result *= factor;
    }
    return result;
}
