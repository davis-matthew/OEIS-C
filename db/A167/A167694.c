/*
 * A167694  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*∑_{k=1}^{14} a(n-k) – 231*a(n‑15) (for n ≥ 16) a(0) =
 *            1, for 1 ≤ n ≤ 14: a(n) = 2 + 21*∑_{k=1}^{n-1} a(n-k); for n =
 *            15: a(15) = 1 + 21*∑_{k=1}^{14} a(15‑k) – 231*a(0);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms of the sequence          */
static idx_t cache_size = 0;      /* current length of the cache array     */
static idx_t computed_max = 0;    /* number of elements already computed   */

static void ensure_size(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    size_t new_size = cache_size ? cache_size : 32;
    while (new_size <= (size_t)n + 1)
        new_size <<= 1;          /* grow geometrically */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

static void init_cache(void)
{
    ensure_size(1);
    cache[0] = 1;                 /* a(0) */
    computed_max = 1;              /* we know a(0) only at this point   */
}

seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    if (n < computed_max)
        return cache[n];

    ensure_size(n);

    for (idx_t i = computed_max; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 14 && i >= k; ++k)
            sum += cache[i - k];

        seq_t term15 = 0;
        if (i >= 15)
            term15 = cache[i - 15];

        seq_t coeff = 0;
        if (i == 0)
            coeff = 1;
        else if (i <= 14)
            coeff = 2;
        else if (i == 15)
            coeff = 1;          /* numerator coefficient for t^15    */

        /* a(i) = coeff + 21*sum − 231*term15    (always non‑negative) */
        seq_t val = coeff + 21 * sum - 231 * term15;
        cache[i] = val;
    }

    computed_max = n + 1;
    return cache[n];
}
