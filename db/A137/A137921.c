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
 * Validated: standard, pass, 121 terms through n = 9915 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static void count_special(idx_t n, size_t idx, idx_t cur, const uint64_t *pr,
                          const uint64_t *ex, size_t cnt, seq_t *special)
{
    if (idx == cnt) {
        if (cur == UINT64_MAX) return;               /* avoid overflow */
        idx_t next = cur + (idx_t)1;
        if (next != 0 && n % next == 0) (*special)++; /* d+1 is a divisor */
        return;
    }
    idx_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        count_special(n, idx + 1, cur * mult, pr, ex, cnt, special);
        if (e < ex[idx]) mult *= pr[idx];
    }
}

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
