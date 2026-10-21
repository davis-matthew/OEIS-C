/*
 * A086971  Number of semiprime divisors of n.
 * https://oeis.org/A086971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(omega(n),2) + #{p : e_p(n) ≥ 2} where n =
 *            ∏p^{e_p(n)} and omega(n) = number of distinct primes of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* arrays large enough for any 64‑bit n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);            /* cnt = number of distinct primes */

    seq_t result = 0;
    if (cnt >= 2) {
        seq_t k = (seq_t)cnt;
        result = k * (k - 1) / 2;           /* binomial(cnt,2) */
    }
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] >= 2)
            result += 1;                    /* prime square divisor */

    return result;
}
