/*
 * A178830  Number of distinct partitions of {1,...,n} into two nonempty sets
 *          P and S with the product of the elements of P equal to the sum of
 *          elements in S.
 * https://oeis.org/A178830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of partitions of {1,…,n} into two nonempty sets
 *            P,S such that product(P) = sum(S). Computation uses enumeration
 *            of subsets of {2,…,n} that satisfy prod+sum = T or T-1, where T
 *            = n*(n+1)/2
 * Range:     u8 n <= 1800, u16 n <= 1800, u32 n <= 1800, u64 n <= 1800,
 *            u128 n <= 1800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static inline void dfs(idx_t n, uint64_t target,
                       idx_t start, uint64_t prod, uint64_t sum,
                       uint64_t *cnt)
{
    for (idx_t i = start; i <= n; ++i) {
        /* stop if multiplying by i would exceed the target */
        if (prod > target / (uint64_t)i)
            break;

        uint64_t new_prod = prod * (uint64_t)i;
        uint64_t new_sum  = sum + (uint64_t)i;

        uint64_t lhs = new_prod + new_sum;
        if (lhs == target)
            ++(*cnt);

        if (lhs < target)
            dfs(n, target, i + 1, new_prod, new_sum, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* total sum of the set {1,…,n} */
    uint64_t T = (uint64_t)n * ((uint64_t)n + 1) / 2;

    uint64_t cnt0 = 0, cnt1 = 0;

    dfs(n, T,        2, 1, 0, &cnt0);      /* case without 1 in P   */
    if (T > 0)                     /* avoid underflow if n=1 */
        dfs(n, T - 1, 2, 1, 0, &cnt1);      /* case with 1 in P       */

    /* The result always fits in seq_t for the supplied build. */
    return (seq_t)(cnt0 + cnt1);
}
