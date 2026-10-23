/*
 * A137921  Number of divisors d of n such that d+1 is not a divisor of n.
 * https://oeis.org/A137921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (#divisors of n) – (#divisors d of n such that d+1
 *            divides n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_special, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/count_special.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for 0, return 0 */

    /* factorize n */
    size_t cnt = 0;
    uint64_t pr[64];
    uint64_t ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* total number of divisors: product (ex[i]+1) */
    seq_t total = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        total *= (seq_t)(ex[i] + 1);

    /* count divisors d where d+1 divides n */
    seq_t special = (seq_t)0;
    count_special(n, 0, (idx_t)1, pr, ex, cnt, &special);

    return total - special;
}
