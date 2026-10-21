/*
 * A080224  Number of abundant divisors of n.
 * https://oeis.org/A080224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) counts the divisors d of n for which σ(d) > 2·d * (i.e., d
 *            is an abundant number). The function factors n, * enumerates all
 *            its divisors, computes the sum of divisors for * each divider
 *            via multiplicative formula, and counts those that * satisfy the
 *            abundance inequality. The algorithm runs i...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"
#include <stdint.h>          /* for uint64_t */

static void dfs_abundant(uint64_t cur, uint64_t sigma,
                         size_t idx, const uint64_t *pr,
                         const uint64_t *exp_arr, size_t cnt,
                         seq_t *count)
{
    if (idx == cnt) {
        if (sigma > (cur << 1))
            (*count)++;
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t pow = 1;               /* p^e */
    for (uint64_t e = 0; e <= exp_arr[idx]; ++e) {
        /* sum of divisors for p^e : (p^(e+1)-1)/(p-1)
           compute using current pow = p^e */
        uint64_t sigma_pe = (pow * p - 1) / (p - 1);

        dfs_abundant(cur * pow, sigma * sigma_pe,
                     idx + 1, pr, exp_arr, cnt, count);

        pow *= p;                   /* next exponent  */
    }
}

seq_t a(idx_t n)
{
    /* Factor the argument using the supplied utility. */
    uint64_t pr[32];
    uint64_t exp_arr[32];
    size_t   cnt = 0;
    factor((uint64_t)n, pr, exp_arr, &cnt);

    seq_t result = 0;
    dfs_abundant(1, 1, 0, pr, exp_arr, cnt, &result);
    return result;
}
