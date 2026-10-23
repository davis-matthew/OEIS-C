/*
 * A170290  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170290:
   a(0) = 1, a(1) = 41
   For 1 ≤ n ≤ 41:  a(n) = p(n) + 39 * Σ_{k=0}^{n-1} a(k),
     where  p(0)=p(41)=1 and p(k)=2 for 1 ≤ k ≤ 40.
   For n ≥ 42:  a(n) = 39 * Σ_{k=n-40}^{n-1} a(k) – 780 * a(n-41). */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of terms currently stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */
    static seq_t sum_all     = 0;   /* sum of cache[0 .. cache_size-1] */

    if (n < cache_size)                /* term already known */
        return cache[(size_t)n];

    /* Ensure sufficient capacity. */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        idx_t new_cap = needed * 2 + 64;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i <= 41) {                                   /* initial values   */
            seq_t p = (i == 0 || i == 41) ? 1 : 2;
            val = p + (seq_t)39 * sum_all;
            cache[(size_t)i] = val;
            sum_all += val;
        } else {                                          /* recurrence part  */
            seq_t sum40 = 0;
            for (idx_t k = 1; k <= 40; ++k)
                sum40 += cache[(size_t)(i - k)];
            val = (seq_t)39 * sum40 - (seq_t)780 * cache[(size_t)(i - 41)];
            cache[(size_t)i] = val;
        }
    }

    cache_size = needed;
    return cache[(size_t)n];
}
