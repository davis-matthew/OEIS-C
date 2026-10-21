/*
 * A213059  Subsets of positive integers arranged in canonical order.
 * https://oeis.org/A213059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A213059: concatenate the elements of the subset of positive integers
   sorted by largest element, then by decreasing size, then lexicographically. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

/* Small binomial coefficient for 0 <= k <= n <= 64, result as idx_t. */
static inline idx_t small_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Unrank combinations of r elements from 1..n (0‑based offset). */
static inline void unrank_combination(idx_t n, idx_t r, idx_t offset, idx_t *comb)
{
    idx_t start = 1;
    for (idx_t i = 0; i < r; ++i)
    {
        for (idx_t val = start; val <= n - (r - i - 1); ++val)
        {
            idx_t cnt = small_binom(n - val, r - i - 1);
            if (offset < cnt)
            {
                comb[i] = val;
                start   = val + 1;
                break;
            }
            offset -= cnt;
        }
    }
}

seq_t a(idx_t n)
{
    /* Find the largest element m such that 2^m - 1 >= n. */
    idx_t pow2 = 1;       /* 2^m */
    idx_t m    = 0;
    while (pow2 - 1 < n)
    {
        pow2 <<= 1;
        ++m;
    }

    /* Offset within the block of subsets having maximum m (1‑based). */
    idx_t prev_total = (pow2 >> 1) - 1;  /* 2^(m-1) - 1 */
    idx_t offset     = n - prev_total;   /* 1‑based position in block */

    /* Determine the size k of the subset. */
    idx_t remaining = offset;   /* 1‑based within current block */
    idx_t k = 0;                /* subset size */
    idx_t r = 0;                /* elements excluding m */

    for (idx_t sz = m; sz >= 1; --sz)
    {
        idx_t cnt = small_binom(m - 1, sz - 1);   /* subsets with size sz */
        if (remaining <= cnt)
        {
            k = sz;
            r = sz - 1;
            break;
        }
        remaining -= cnt;
    }

    /* Now unrank the combination of r elements chosen from 1..m-1. */
    idx_t comb[32] = {0};
    if (r > 0)
        unrank_combination(m - 1, r, remaining - 1, comb);

    /* Build the concatenated integer. */
    seq_t res = 0;
    if (r > 0)
    {
        for (idx_t i = 0; i < r; ++i)
        {
            seq_t val  = (seq_t)comb[i];
            seq_t pow  = pow10(digits((idx_t)comb[i]));
            res = res * pow + val;
        }
    }

    /* Append the largest element m. */
    seq_t powm = pow10(digits(m));
    res = res * powm + (seq_t)m;
    return res;
}
