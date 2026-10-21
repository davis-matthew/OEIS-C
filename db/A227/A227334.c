/*
 * A227334  Exponent of the group of the Gaussian integers in a reduced system
 *          modulo n.
 * https://oeis.org/A227334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the multiplicative group of Gaussian integers
 *            modulo n (i.e. lcm of the exponents of Gaussian integer residue
 *            class groups for each prime power divisor of n). a(1) = 1
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n <= 1) return 1;

    /* factor n; max 64 distinct prime factors for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t comp;

        if (p == 2) {
            /* a(2^e) = 2^e (e≤2) or 2^(e-1) (e≥3) */
            if (e <= 2)
                comp = pow_uint(2, e);
            else
                comp = pow_uint(2, e - 1);
        } else {
            seq_t powpe_1 = pow_uint(p, e - 1);
            if (p % 4 == 1) {
                /* a(p^e) = (p-1)*p^(e-1) for p ≡ 1 (mod 4) */
                comp = (seq_t)(p - 1) * powpe_1;
            } else { /* p % 4 == 3 */
                /* a(p^e) = (p^2-1)*p^(e-1) for p ≡ 3 (mod 4) */
                comp = ((seq_t)(p - 1) * (seq_t)(p + 1)) * powpe_1;
            }
        }

        seq_t g = gcd(result, comp);
        result = (result / g) * comp;   /* lcm of result and comp */
    }
    return result;
}
