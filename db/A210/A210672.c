/*
 * A210672  a(0)=1; thereafter a(n) = 2*Sum_{k=1..n} binomial(2n,2k)*a(n-k).
 * https://oeis.org/A210672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210672
 * a(0) = 1
 * a(n) = 2 * Σ_{k=1..n} binomial(2n, 2k) * a(n-k)   (n ≥ 1)
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Make sure the cache holds indices up to n. */
    ensure_cache(n, &cache, &cache_len);

    if (cache[0] == (seq_t)0)
        cache[0] = 1;                 /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0)
            continue;                 /* already computed */

        seq_t sum = 0;
        seq_t bin = 1;                /* C(2*i, 0) */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* Update bin to C(2*i, 2k) from C(2*i, 2k-2) */
            idx_t num1 = (idx_t)(2 * i - 2 * k + 2);   /* 2i‑2k+2 */
            idx_t num2 = (idx_t)(2 * i - 2 * k + 1);   /* 2i‑2k+1 */
            idx_t den  = (idx_t)(2 * k * (2 * k - 1)); /* 2k*(2k‑1) */

            bin = mul_small(bin, (unsigned)num1);
            bin = mul_small(bin, (unsigned)num2);
            bin = bin / (seq_t)den;

            sum += bin * cache[i - k];
        }

        cache[i] = sum * 2;
    }

    return cache[n];
}
