/*
 * A060968  Number of solutions to x^2 + y^2 == 1 (mod n).
 * https://oeis.org/A060968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * ∏_{p|n, p≡1 (mod 4)} (1 - 1/p) * ∏_{p|n, p≡3 (mod 4)}
 *            (1 + 1/p) * (1 + [4|n])
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the equivalent multiplicative rules: */

/* a(2^e) = 2          (e = 1)
 *            = 2^(e+1) (e > 1)
 * For odd prime powers
 *            a(p^e) = (p-1)·p^(e-1)   if p ≡ 1 (mod 4)
 *            a(p^e) = (p+1)·p^(e-1)   if p ≡ 3 (mod 4) */

#include <stdint.h>
#include "../utils/factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factor n into its prime powers */
    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    factor((uint64_t)n, pr, exp, &cnt);  /* static helper from factor.h */

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = exp[i];
        seq_t part;

        if (p == 2) {                /* power of 2 */
            if (e == 1)
                part = (seq_t)2;
            else {
                /* 2^(e+1) */
                part = (seq_t)1;
                for (uint64_t j = 0; j < e + 1; ++j)
                    part *= (seq_t)2;
            }
        } else {
            /* compute p^(e-1) */
            seq_t ppow = (seq_t)1;
            for (uint64_t j = 1; j < e; ++j)
                ppow *= (seq_t)p;

            if ((p & 3ULL) == 1ULL)     /* p ≡ 1 (mod 4) */
                part = (seq_t)(p - 1) * ppow;
            else                        /* p ≡ 3 (mod 4) */
                part = (seq_t)(p + 1) * ppow;
        }

        result *= part;
    }

    return result;
}
