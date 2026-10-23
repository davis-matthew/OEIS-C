/*
 * A126833  Ramanujan numbers (A000594) read mod 25.
 * https://oeis.org/A126833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n * σ₉(n)) mod 25, with σ₉(n) = Σ_{d|n} d⁹
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute σ₉(n) modulo 25. */
static seq_t sigma9_mod_25(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    uint8_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* p⁹ mod 25 */
        uint8_t pmod = (uint8_t)(p % 25);
        uint8_t p9 = 1;
        for (int j = 0; j < 9; ++j)
            p9 = (uint8_t)((p9 * pmod) % 25);

        /* Σ_{k=0..e} (p9)^k  (mod 25) */
        uint8_t sum  = 1;   /* term for k = 0 */
        uint8_t term = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            term = (uint8_t)((term * p9) % 25);
            sum  = (uint8_t)((sum + term) % 25);
        }

        result = (uint8_t)((result * sum) % 25);
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Reduce n and σ₉(n) modulo 25 before the final multiplication. */
    uint64_t n_mod      = (uint64_t)(n % 25);
    uint64_t sigma_mod  = (uint64_t)sigma9_mod_25((uint64_t)n);

    return (seq_t)((n_mod * sigma_mod) % 25);
}
