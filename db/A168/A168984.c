/*
 * A168984  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence of order 23 a(0) = 1 a(n) for 1 ≤ n ≤ 22
 *            computed from a(n) = 2 + 17 * (a(n-1)+a(n-2)+…+a(0)) a(23)
 *            computed from a(23) = 1 + 17 * (a(22)+…+a(1)) – 153 * a(0) For n
 *            ≥ 24 a(n) = 17 * (a(n-1)+a(n-2)+…+a(n-22)) – 153 * a(n-23)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache holds all values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
    if (!new_cache) exit(1);                     /* allocation failure */

    /* copy old values */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    if (cache) free(cache);
    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = new_size;

    /* Compute missing terms */
    for (idx_t k = old_size; k <= n; ++k)
    {
        if (k == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (k <= 22)               /* 1 … 22 */
        {
            seq_t term = (seq_t)2;           /* N_k for k ≥ 1 */
            for (idx_t i = 1; i <= k; ++i)
                term += (seq_t)17 * cache[k - i];
            cache[k] = term;
        }
        else if (k == 23)                /* special case */
        {
            seq_t term = (seq_t)1;           /* N_23 */
            for (idx_t i = 1; i <= 22; ++i)
                term += (seq_t)17 * cache[23 - i];
            term -= (seq_t)153 * cache[0];
            cache[23] = term;
        }
        else                             /* n ≥ 24 */
        {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 22; ++i)
                sum += (seq_t)17 * cache[k - i];
            sum -= (seq_t)153 * cache[k - 23];
            cache[k] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
