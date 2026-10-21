/*
 * A204386  Expansion of (theta_2(q)^8 + 4 * theta_2(q^2)^8) / 256 in powers
 *          of q^2.
 * https://oeis.org/A204386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative sequence a(2^e) = 3 * 2^(3e-1) for e > 0 a(p^e) =
 *            1 + p^3 + p^6 + ... + p^(3e) for odd prime p a(n) = product of
 *            the above for all prime powers in n Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t, size_t */
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, and a(0) is unused */

    uint64_t primes[64], exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = primes[k];
        uint64_t e = exps[k];

        if (p == 2) {                    /* special case for 2 */
            idx_t exp = (idx_t)(3 * e - 1);
            seq_t pow2 = seq_pow((seq_t)2, exp);
            seq_t part = pow2 * (seq_t)3;
            result *= part;
        } else {                         /* odd prime */
            seq_t pseq = (seq_t)p;
            seq_t p3   = pseq * pseq * pseq;     /* p^3 */
            seq_t pk   = 1;      /* current term p^(3i) */
            seq_t part  = 1;     /* 1 + p^3 + ... */

            for (idx_t i = 1; i <= (idx_t)e; ++i) {
                pk *= p3;
                part += pk;
            }
            result *= part;
        }
    }
    return result;
}
