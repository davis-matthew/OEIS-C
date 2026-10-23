/*
 * A125096  Expansion of -1 + (phi(q) * phi(q^2) + phi(-q^2) * phi(q^4)) / 2
 *          in powers of q.
 * https://oeis.org/A125096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is defined multiplicatively: - a(1) = 1 - For 2^e: a(2) =
 *            0, a(2^e) = 2 for e > 1 - For odd primes p: if p ≡ 1 or 3 (mod
 *            8): a(p^e) = e+1 if p ≡ 5 or 7 (mod 8): a(p^e) = 1 if e even, 0
 *            if e odd The value for n is the product of a(p^e) over all prime
 *            powers of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    /* Allocate space for prime factors and exponents.
       64 is more than enough for 64‑bit numbers. */
    uint64_t primes[64];
    size_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = primes[i];
        size_t exp = exps[i];

        seq_t factor = 1;

        if (prime == 2ULL) {
            if (exp == 1) return 0;
            /* exp > 1 */
            factor = 2;
        } else {
            uint8_t mod8 = prime & 7U; /* prime % 8 */
            if (mod8 == 1U || mod8 == 3U) {
                /* e+1 */
                factor = (seq_t)(exp + 1);
            } else { /* mod8 == 5 or 7 */
                if ((exp & 1U) == 0U) {
                    factor = 1;
                } else {
                    return 0;
                }
            }
        }
        result *= factor;
    }

    return result;
}
