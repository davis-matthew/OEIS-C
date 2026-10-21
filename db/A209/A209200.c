/*
 * A209200  G.f.: (1-4*x)^(-1/2) * (1-8*x)^(-1/4).
 * https://oeis.org/A209200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*(n-k), n-k) * A004981(k)
 *            Recurrence: n * a(n) = 4 * (3*n-2) * a(n-1) - 8 * (4*n-5) *
 *            a(n-2) Initial values: a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i < cache_size. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Return cached value if already computed. */
    if (n < cache_size)
        return cache[n];

    /* Extend cache to hold at least n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;           /* Allocation failure – safe fallback. */
    cache = tmp;

    /* Initialize base terms when missing. */
    if (cache_size == 0)
        cache[0] = (seq_t)1;
    if (new_size > 1 && cache_size < 2)
        cache[1] = (seq_t)4;

    /* Compute coefficients and terms starting at the first new index. */
    idx_t start = (cache_size >= 2) ? (idx_t)cache_size : 2;

    for (idx_t i = start; i <= n; ++i)
    {
#ifdef __cplusplus
        /* Use native operators: the arbitrary‑precision class handles
           huge intermediate values. */
        seq_t term1 = cache[i-1] * (seq_t)(4 * ((seq_t)3*i - 2));
        seq_t term2 = cache[i-2] * (seq_t)(8 * ((seq_t)4*i - 5));
        cache[i] = (term1 - term2) / (seq_t)i;
#else
        /* Built‑in integer build: use 128‑bit intermediate arithmetic
           to avoid overflow. */
        __uint128_t coeff1 = (__uint128_t)4 * (((__uint128_t)3 * i) - 2);
        __uint128_t coeff2 = (__uint128_t)8 * (((__uint128_t)4 * i) - 5);

        __uint128_t t1 = (__uint128_t)cache[i-1] * coeff1;
        __uint128_t t2 = (__uint128_t)cache[i-2] * coeff2;

        cache[i] = (seq_t)((t1 - t2) / i);
#endif
    }

    cache_size = new_size;
    return cache[n];
}
