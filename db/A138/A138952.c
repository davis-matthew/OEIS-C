/*
 * A138952  Expansion of (eta(q^2)^7 * eta(q^3)^2 * eta(q^12) / (eta(q)^2 *
 *          eta(q^4)^3 * eta(q^6)^3) - 1) / 2 in powers of q.
 * https://oeis.org/A138952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Multiplicative sequence defined by local prime‑power rules
 *            a(2^e) = -1 (e>0) a(3^e) = 1 if e even, -3 if e odd a(p^e) = e+1
 *            if p ≡ 1 or 5 (mod 12) a(p^e) = (1+(-1)^e)/2 if p ≡ 7 or 11 (mod
 *            12) a(1) = 1 a(n) = product over its prime powers
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* assumes declaration of factor_u64 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* arrays for prime factors and exponents; sufficient for n up to 64 bits */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t factor;

        if (p == 2) {
            factor = (seq_t)-1;
        } else if (p == 3) {
            factor = (e & 1) ? (seq_t)-3 : (seq_t)1;
        } else {
            uint64_t mod12 = p % 12;
            if (mod12 == 1 || mod12 == 5) {
                factor = (seq_t)(e + 1);
            } else { /* mod12 == 7 or 11 */
                factor = ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
            }
        }

        if (factor == (seq_t)0) return (seq_t)0;
        result *= factor;
    }
    return result;
}
