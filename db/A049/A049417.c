/*
 * A049417  a(n) = isigma(n): sum of infinitary divisors of n.
 * https://oeis.org/A049417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of infinitary divisors of n. Equivalent
 *            multiplicative form: for n = ∏p^e, a(p^e) = ∏_{k : bit k of e is
 *            1} (p^(2^k) + 1)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper to factor an unsigned 64‑bit integer */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* offset safety, not used by OEIS calls */
        return 0;

    uint64_t m = (uint64_t)n;              /* n fits in uint64_t */
    uint64_t pr[64], ex[64];               /* enough for any 64‑bit n */
    size_t   cnt;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t   e   = ex[i];
        seq_t      term = 1;
        uint64_t   bit  = 1;               /* current power of two 2^k */

        while (e)
        {
            if (e & 1ULL)                  /* this bit of e is set */
            {
                seq_t pow = 1;
                for (uint64_t j = 0; j < bit; ++j)
                    pow *= (seq_t)pr[i];
                term *= (pow + (seq_t)1);
            }
            e >>= 1;
            bit <<= 1;                      /* advance to next 2^k */
        }
        result *= term;
    }
    return result;
}
