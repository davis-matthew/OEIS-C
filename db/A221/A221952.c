/*
 * A221952  Number of subgroups of C_5 X C_n.
 * https://oeis.org/A221952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6·ν₅(n) + 2) · τ(n / 5^{ν₅(n)})
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ν₅(n) is the exponent of 5 in n and τ(m) is the number of
   positive divisors of m.  The group C5 × Cn is cyclic iff 5∤n,
   yielding 2·τ(n).  If 5|n, the 5‑Sylow part is C5 × C_{5^k},
   which has 6k+2 subgroups.  The total number of subgroups equals
   (6k+2)·τ(m). */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64() */

/* compute τ(m) for m ≥ 1 */
static seq_t divisor_count_u64(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
        res *= (seq_t)(exps[i] + 1);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the offset, harmless */

    idx_t k = 0;                   /* exponent of 5 in n */
    uint64_t temp = (uint64_t)n;   /* local copy for division */
    while ((temp % 5ULL) == 0ULL) {
        temp /= 5ULL;
        ++k;
    }

    /* m = n / 5^k, coprime to 5 */
    seq_t tau = divisor_count_u64(temp);

    /* factor = 6*k + 2; compute safely in seq_t */
    seq_t factor = (seq_t)(6 * (seq_t)k + 2);

    return factor * tau;
}
