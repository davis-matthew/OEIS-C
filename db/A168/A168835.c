/*
 * A168835  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence from the rational generating function (1 - 12 t - …
 *            -12 t¹⁹ + 78 t²⁰) * A(t) = (1 + 2 t + … + 2 t¹⁹ + t²⁰) a(0) = 1
 *            for 1 ≤ n ≤ 19: a(n) = 2 + 12 * Σ_{i=1}^{n} a(n-i) a(20) = 12 *
 *            Σ_{i=1}^{19} a(20-i) - 78 * a(0) + 1 for n > 20: a(n) = 12 *
 *            Σ_{i=1}^{19} a(n-i) - 78 * a(n-20)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* dynamic array of previously computed values   */
    static size_t  cache_size = 0;       /* current capacity of cache                    */
    static idx_t   last_computed = (idx_t)-1;  /* index of the largest computed term          */

    /* grow the cache if necessary                                */
    if (n + 1 > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* out‑of‑memory → return 0 but callers may check */
        cache = tmp;
        cache_size = n + 1;
    }

    /* compute new terms one by one                                   */
    for (idx_t k = last_computed + 1; k <= n; ++k)
    {
        if (k == 0)
        {
            cache[0] = (seq_t)1;                          /* a(0) = 1 */
            continue;
        }

        /* terms 1 … 19 : a(k) = 2 + 12 * Σ a(k-i)          */
        if (k <= 19)
        {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= k; ++i) sum += cache[k - i];
            cache[k] = (seq_t)2 + ((seq_t)12 * sum);
            continue;
        }

        /* term 20 : special RHS (equals 1)                      */
        if (k == 20)
        {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= 19; ++i) sum += cache[20 - i];
            cache[20] = (seq_t)12 * sum - ((seq_t)78 * cache[0]) + (seq_t)1;
            continue;
        }

        /* general recurrence for n > 20                          */
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= 19; ++i) sum += cache[k - i];
        cache[k] = (seq_t)12 * sum - ((seq_t)78 * cache[k - 20]);
    }

    last_computed = (idx_t) (last_computed > n ? last_computed : n);
    return cache[n];
}
