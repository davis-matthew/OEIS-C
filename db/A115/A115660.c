/*
 * A115660  Expansion of (phi(q) * phi(q^6) - phi(q^2) * phi(q^3)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A115660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Multiplicative definition: * a(1) = 1. * For each prime power
 *            p^e in n: * - if p == 2 or 3: a(p^e) = (-1)^e * - if p ≡ 1 or 7
 *            (mod 24): a(p^e) = e + 1 * - if p ≡ 5 or 11 (mod 24): a(p^e) =
 *            (e + 1) * (-1)^e * - if p ≡ 13, 17, 19, 23 (mod 24): a(p^e) = (1
 *            + (-1)^e)/2 * a(n) = product of all a(p^e)...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t local = (seq_t)1;

        if (p == 2ULL || p == 3ULL) {
            local = (e & 1ULL) ? (seq_t)-1 : (seq_t)1;
        } else {
            uint64_t r = p % 24ULL;
            if (r == 1ULL || r == 7ULL) {
                local = (seq_t)(e + 1ULL);
            } else if (r == 5ULL || r == 11ULL) {
                seq_t t = (seq_t)(e + 1ULL);
                if (e & 1ULL) t = -t;
                local = t;
            } else if (r == 13ULL || r == 17ULL || r == 19ULL || r == 23ULL) {
                local = (e & 1ULL) ? (seq_t)0 : (seq_t)1;
            } else {
                /* all residues modulo 24 are covered above */
                local = (seq_t)1;
            }
        }

        if (local == (seq_t)0) {
            result = (seq_t)0;
            break;
        }
        result *= local;
    }

    return result;
}
