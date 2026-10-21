/*
 * A069093  Jordan function J_8(n).
 * https://oeis.org/A069093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (p^(8e) - p^(8(e-1))) (Jordan J_8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, ipow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_seq_t.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1U)
        return (seq_t)1;

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t  e = (idx_t)exps[i];

        seq_t term = ipow_seq_t(p, 8 * e)          /* p^(8e) */
                  - ipow_seq_t(p, 8 * (e - 1));   /* p^(8(e-1)) */

        result *= term;
    }

    return result;
}
