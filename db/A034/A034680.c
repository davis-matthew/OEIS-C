/*
 * A034680  Sum of sixth powers of unitary divisors.
 * https://oeis.org/A034680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^k || n} (1 + p^{6*k}) (sum of sixth powers of
 *            unitary divisors)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1621,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* safety: offset is 1 */

    uint64_t nn = (uint64_t)n;
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p      = primes[i];
        uint64_t k      = exps[i];
        idx_t    expo   = (idx_t)(6ULL * k);   /* 6*k fits into idx_t for 64-bit n */

        seq_t powv = pow_uint(p, expo);   /* p^(6*k) in seq_t */
        powv += 1;                        /* 1 + p^(6*k) */
        result *= powv;                   /* product over all prime powers */
    }

    return result;
}
