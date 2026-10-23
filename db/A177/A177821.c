/*
 * A177821  a(n) gives the number of nonisomorphic connected compact Lie
 *          groups of dimension n which are simple products.
 * https://oeis.org/A177821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonisomorphic connected compact Lie groups of
 *            dimension n * that are direct products of circle groups U(1) and
 *            simple compact Lie groups. * The generating function is * 1 / (
 *            (1 - x) * Π_{k≥1} (1 - x^k)^{m_k} ) * where m_k is the number of
 *            distinct simple Lie groups of dimension...
 * Range:     u8 n <= 24, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility function from the build environment */
#include "../utils/seq_one.h"

/* Return the number of divisors of n (for n up to 10000). */
static idx_t divisor_count(idx_t n)
{
    idx_t cnt = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (temp > 1)
        cnt *= 2;
    return cnt;
}

/* Cached result table for a(n). */
static seq_t *dp_cache = NULL;
static idx_t dp_cache_limit = 0;

/* Compute a(n) for all n up to limit and store in dp_cache. */
static void compute_cache(idx_t limit)
{
    /* Allocate temporary array for multiplicities m_k. */
    idx_t *m = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!m) return;                 /* allocation failure -> no change */

    /* m[1] = 1  (U(1) factor). */
    if (limit >= 1) m[1] = 1;

    /* Type A_n: SU(n) with n ≥ 2, dimension n^2 - 1, center Z_n.  */
    for (idx_t n = 2; ; ++n)
    {
        idx_t dim = n * n - 1;
        if (dim > limit) break;
        m[dim] += divisor_count(n);    /* number of divisors of n */
    }

    /* Type B_n: Spin(2n+1) with n ≥ 2, centre Z_2.  */
    for (idx_t n = 2; ; ++n)
    {
        idx_t dim = n * (2 * n + 1);
        if (dim > limit) break;
        m[dim] += 2;
    }

    /* Type C_n: Sp(n) with n ≥ 3, centre Z_2.  */
    for (idx_t n = 3; ; ++n)
    {
        idx_t dim = n * (2 * n + 1);
        if (dim > limit) break;
        m[dim] += 2;
    }

    /* Type D_n: Spin(2n) with n ≥ 4. */
    for (idx_t n = 4; ; ++n)
    {
        idx_t dim = n * (2 * n - 1);
        if (dim > limit) break;
        if (n == 4)               m[dim] += 3;            /* triality case */
        else if (n % 2 == 1)      m[dim] += 3;            /* centre Z_4 */
        else                      m[dim] += 5;            /* centre Z_2 × Z_2 */
    }

    /* Exceptional groups.  */
    if (limit >= 14)  m[14]  += 1;  /* G2 */
    if (limit >= 52)  m[52]  += 1;  /* F4 */
    if (limit >= 78)  m[78]  += 2;  /* E6 */
    if (limit >=133)  m[133] += 2;  /* E7 */
    if (limit >=248)  m[248] += 1;  /* E8 */

    /* Count total number of simple group instances (with multiplicity). */
    size_t group_cnt = 0;
    for (idx_t k = 1; k <= limit; ++k)
    {
        if (m[k] > 0) group_cnt += (size_t)m[k];
    }

    /* Build array of individual group dimensions. */
    idx_t *groups = (idx_t *)malloc(group_cnt * sizeof(idx_t));
    if (!groups)
    {
        free(m);
        return;
    }
    size_t pos = 0;
    for (idx_t k = 1; k <= limit; ++k)
    {
        for (idx_t t = 0; t < m[k]; ++t)
            groups[pos++] = k;
    }

    /* Recompute DP array. */
    seq_t *new_dp = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_dp)
    {
        free(m);
        free(groups);
        return;
    }

    new_dp[0] = seq_one();   /* one way to build dimension 0 (empty product) */

    /* Unbounded knapsack: for each simple group type, add it unlimited times. */
    for (size_t g = 0; g < group_cnt; ++g)
    {
        idx_t d = groups[g];
        for (idx_t i = d; i <= limit; ++i)
        {
            new_dp[i] = new_dp[i] + new_dp[i - d];
        }
    }

    /* Replace old cache. */
    free(dp_cache);
    dp_cache = new_dp;
    dp_cache_limit = limit;

    free(m);
    free(groups);
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (dp_cache == NULL || n > dp_cache_limit)
        compute_cache(n);

    return dp_cache[n];
}
