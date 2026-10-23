/*
 * A160889  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 4.
 * https://oeis.org/A160889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^2 + p + 1) * p^(2e-2) (multiplicative
 *            definition)
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, mul_pow_seq_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mul_pow_seq_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 0 is irrelevant, but safe */
    if (n == 1)
        return 1;

    uint64_t n64 = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n64, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t part = p2 + (seq_t)p + 1;
        if (e > 1)
        {
            idx_t exp = (idx_t)(2 * (e - 1));
            seq_t powp = mul_pow_seq_uint((seq_t)p, exp);
            part *= powp;
        }
        result *= part;
    }
    return result;
}
