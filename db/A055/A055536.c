/*
 * A055536  Number of asymmetric types of (3,n)-hypergraphs under action of
 *          symmetric group S_3.
 * https://oeis.org/A055536
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/6 * [ binom(n+7,7) - 3 * Σ_{t=0}^{⌊n/2⌋}
 *            binom(n-2t+3,3) * (t+1) + 2 * Σ_{q=0}^{⌊n/3⌋} (n-3q+1) * (q+1) ]
 *            for n ≥ 2 (offset is 2)
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* holds computed a(n) values */
static idx_t  cached_len = 0;        /* number of entries stored (max index + 1) */

/* compute binomial(x,7) = x*(x-1)*...*(x-6)/5040  */
static inline seq_t binom7(idx_t x)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k < 7; ++k)
        prod *= (seq_t)(x - k);
    return prod / (seq_t)5040;
}

/* compute binomial(x,3) = x*(x-1)*(x-2)/6  */
static inline seq_t binom3(idx_t x)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k < 3; ++k)
        prod *= (seq_t)(x - k);
    return prod / (seq_t)6;
}

/* Ensure cache has values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cached_len > n)
        return; /* already computed */

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;

    for (idx_t i = cached_len; i <= n; ++i)
    {
        /* term a1 = binom(i+7,7) */
        seq_t term_a1 = binom7(i + 7);

        /* sum_B = Σ_{t=0}^{⌊i/2⌋} binom(i-2t+3,3) * (t+1) */
        seq_t sum_B = (seq_t)0;
        for (idx_t t = 0; (t << 1) <= i; ++t)
        {
            idx_t m = i - (t << 1);
            seq_t add = binom3(m + 3) * ((seq_t)(t + 1));
            sum_B += add;
        }

        /* sum_C = Σ_{q=0}^{⌊i/3⌋} (i-3q+1) * (q+1) */
        seq_t sum_C = (seq_t)0;
        for (idx_t q = 0; (q * 3) <= i; ++q)
        {
            idx_t p = i - (q * 3);
            seq_t add = ((seq_t)(p + 1)) * ((seq_t)(q + 1));
            sum_C += add;
        }

        seq_t numerator = term_a1 - (seq_t)3 * sum_B + (seq_t)2 * sum_C;
        cache[i] = numerator / (seq_t)6;
    }

    cached_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* values below the offset are undefined */
    ensure_cache(n);
    return cache[n];
}
