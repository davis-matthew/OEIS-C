/*
 * A209979  Number of unimodular 2 X 2 matrices having all elements in
 *          {1,2,...,n}.
 * https://oeis.org/A209979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1}^{n²-1} g(k) * g(k+1) where g(k) = #{(x, y) ∈
 *            {1,…,n}² : x·y = k} (ordered pairs)
 * Range:     u8 n <= 10, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(0) = a(1) = 0 */
        return 0;

    /* Number of distinct product values: 0 … n²+1  (inclusive) */
    size_t prod_max = (size_t)n * (size_t)n + 2;
    uint16_t *g = (uint16_t *)calloc(prod_max, sizeof(uint16_t));
    if (!g) return 0;         /* unlikely in the test harness */

    seq_t sum_adj = 0;        /* Σ g(k) * g(k+1) */

    for (idx_t a = 1; a <= n; ++a)
    {
        for (idx_t d = 1; d <= n; ++d)
        {
            idx_t p = a * d;          /* 1 ≤ p ≤ n²  */

            /* Increment the adjacency sum by current neighbours of p */
            sum_adj += (seq_t)g[p - 1] + (seq_t)g[p + 1];

            ++g[p];                   /* update count of product p */
        }
    }

    free(g);
    return sum_adj * 2;               /* a(n) = 2 * Σ g(k) * g(k+1) */
}
