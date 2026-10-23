/*
 * A159076  A008474(n) + 2.
 * https://oeis.org/A159076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2 + Σ_{p^e‖n} (p + e) (the factor 1^1 is included
 *            implicitly, giving the initial 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* defers to caller: indices start at 1 */
        return 0;

    /* 1^1 contributes 1+1 = 2 */
    seq_t total = 2;

    /* factor_u64 returns up to 64 prime factors for a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
        total += (seq_t)primes[i] + (seq_t)exps[i];

    return total;
}
