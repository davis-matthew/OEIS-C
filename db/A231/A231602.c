/*
 * A231602  Triangular array read by rows: T(n,k) is the number of rooted
 *          labeled trees on n nodes that have exactly k nodes with outdegree
 *          = 1, n>=1, 0<=k<=n-1.
 * https://oeis.org/A231602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 58, u64 n <= 155,
 *            u128 n <= 410, big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231602: Number of rooted labeled trees on n nodes with exactly k nodes
   of outdegree 1.  This function returns the terms of the flattened triangle
   read by rows. */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit integers */
static idx_t isqrt_u64(idx_t x)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62; /* largest power of 4 ≤ 2⁶⁴ */
    while (bit)
    {
        if (res + bit <= x)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Compute T(n,k) for 1 ≤ n and 0 ≤ k ≤ n−1 */
static seq_t trees_with_k_1(idx_t n, idx_t k)
{
    if (k > n - 1) return 0;
    if (n == 1) return 1;

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    seq_t prod = 1;          /* prod = ∏_{t=i}^{n-1} t  (initially for i = n) */

    for (idx_t i = n; i >= 1; --i)
    {
        if (i <= n - k)
        {
            seq_t bin1   = binom(n, i);
            seq_t bin2   = binom(n - i, k);
            seq_t pow_val = big_pow((seq_t)i, (seq_t)(i - 1));
            seq_t term   = prod;
            term = term * bin1;
            term = term * bin2;
            term = term * pow_val;

            /* sign = (-1)^(n - i - k) */
            if (((n - i - k) & 1) == 0)
                pos_sum = pos_sum + term;
            else
                neg_sum = neg_sum + term;
        }

        if (i > 1)
            prod = prod * (seq_t)(i - 1);
    }

    return pos_sum - neg_sum;
}

/* Public entry point required by the test harness */
seq_t a(idx_t n)
{
    /* Determine the row (r) and column (k) corresponding to index n
       in the flattened triangle: indices start at 1. */
    if (n == 0) return 0;   /* undefined for n < 1, but guard silently */

    /* Find smallest r with r(r+1)/2 ≥ n */
    idx_t r = (isqrt_u64(8ULL * n + 1) - 1) / 2;
    while (r * (r + 1) / 2 < n) r++;

    idx_t prev = r * (r - 1) / 2;
    idx_t k    = n - prev - 1;  /* 0 ≤ k ≤ r − 1 */

    return trees_with_k_1(r, k);
}
