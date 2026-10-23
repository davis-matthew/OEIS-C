/*
 * A036879  If n = (p_1)^(m_1)...(p_k)^(m_k) then a(n) = (p_1)^((p_1)^(m_1) -
 *          1)...(p_k)((p_k)^(m_k) - 1).
 * https://oeis.org/A036879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^m‖n} p^(p^m – 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Factor n into primes: p_i^m_i */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        /* Compute p^m */
        seq_t p_pow_m = pow_uint(pr[i], ex[i]);

        /* exponent for the final power: p^m – 1 */
        seq_t exp = p_pow_m - 1;

        /* Compute p^(p^m – 1) */
        seq_t part = big_pow((seq_t)pr[i], exp);

        /* Accumulate the product */
        result *= part;
    }

    return result;
}
