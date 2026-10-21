/*
 * A202411  a(n) = Sum_{k=floor(n/4)..R} C(k, m*k - (-1)^n*(R - k)) * C(k + 1,
 *          m*(k + 2) - (-1)^n*(R - k + 1)) where m = (n + 1) mod 2 and R = (n
 *          + m - 3)/2 for n > 0 and a(0) = 1.
 * https://oeis.org/A202411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=floor(n/4)..R} binom(k, m*k - (-1)^n*(R-k)) *
 *            binom(k+1, m*(k+2) - (-1)^n*(R-k+1)) where m = (n+1) mod 2, R =
 *            (n+m-3)/2 (for n>0) and a(0)=1
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/binom.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;   /* last index that has been computed */

/* compute a single value of the sequence (without memoisation) */
static seq_t compute_a_value(idx_t n)
{
    if (n == 0) return 1;

    int64_t n64 = (int64_t)n;
    int64_t m   = (n64 + 1) & 1;          /* 0 or 1 */
    int64_t sign = (n64 & 1) ? -1 : 1;    /* (-1)^n */
    int64_t R    = (n64 + m - 3) / 2;
    int64_t k_start = n64 / 4;

    if (R < k_start) return 0;

    seq_t sum = 0;
    for (int64_t k = k_start; k <= R; ++k)
    {
        int64_t d1 = m * k - sign * (R - k);
        int64_t d2 = m * (k + 2) - sign * (R - k + 1);

        if (d1 < 0 || d1 > k) continue;
        if (d2 < 0 || d2 > k + 1) continue;

        seq_t b1 = binom((idx_t)k, (idx_t)d1);
        seq_t b2 = binom((idx_t)k + 1, (idx_t)d2);
        sum += b1 * b2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Initialise cache lazily */
    if (cache == NULL)
    {
        grow_seq(&cache, &cache_cap, n + 1);
        cache[0] = 1;
        cache_len = 0;   /* last computed index */
    }
    else if (n >= cache_cap)
    {
        grow_seq(&cache, &cache_cap, n + 1);
    }

    if (n <= cache_len) return cache[n];

    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = compute_a_value(i);

    cache_len = n;
    return cache[n];
}
