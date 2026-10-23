/*
 * A034690  Sum of digits of all the divisors of n.
 * https://oeis.org/A034690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of digit sums of all divisors of n
 * Range:     u8 n <= 651, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_sum.h"

static void dfs(size_t idx, const uint64_t *pr, const uint64_t *ex, size_t cnt,
                seq_t cur, seq_t *sum)
{
    if (idx == cnt) {
        *sum += digit_sum(cur);
        return;
    }
    seq_t cur2 = cur;
    for (uint64_t k = 0; k <= ex[idx]; ++k) {
        dfs(idx + 1, pr, ex, cnt, cur2, sum);
        cur2 *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t nn = (uint64_t)n;

    static uint64_t pr[64];
    static uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t sum = 0;
    dfs(0, pr, ex, cnt, (seq_t)1, &sum);
    return sum;
}
