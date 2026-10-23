/*
 * A169235  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function is G(t) = (t^28 + 2t^27 + 2t^26 + ... +
 *            2t + 1) / (406t^28 - 28t^27 - 28t^26 - ... - 28t + 1). Hence
 *            a(0)=1 and for n≥1 the coefficients satisfy a(n) = b_n +
 *            28·∑_{k=n-27}^{n-1} a(k) - 406·a(n-28), (1) where b_n = 1 for
 *            n=0 or n=28, b_n = 2 for 1≤n≤27, and b_n = 0 for n>28...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache         = NULL;   /* array of terms a(0)..a(k) */
    static size_t cache_cap     = 0;    /* allocated capacity */
    static size_t cache_size     = 0;    /* number of terms stored */

    /* Ensure that the array can address index n */
    ensure_cache(n, &cache, &cache_cap);

    /* a(0) is always present when we have any cache entry */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* If the requested term is already cached, just return it */
    if (n < cache_size)
        return cache[n];

    /* -----------------------------------------------------------------
       Compute terms 1 .. 27 using the simple recurrence
       a(n) = b_n + 28 * sum_{k=0}^{n-1} a(k)   (for n≤27)
       ----------------------------------------------------------------- */
    if (cache_size <= 27 && n >= cache_size) {
        /* cumulative sum of all terms computed so far (a[0]+...+a[i-1]) */
        seq_t cum = cache[0];
        for (idx_t i = 1; i <= n && i <= 27; ++i) {
            /* b_i = 2 for 1≤i≤27 */
            seq_t term = (seq_t)2 + mul_small(cum, 28);
            cache[i] = term;
            cum += term;
        }
        cache_size = (n >= 27) ? 28 : n + 1;
    }

    /* If n is still ≤27, return the cached value */
    if (n < cache_size)
        return cache[n];

    /* -----------------------------------------------------------------
       Compute term 28 using the exact recurrence that includes b_28=1
       ----------------------------------------------------------------- */
    if (cache_size <= 28 && n >= 28) {
        /* Sum of a[1]..a[27] = (sum of a[0]..a[27]) - a[0] */
        seq_t sum_last27 = (cache[27] + cache[0] - cache[0]); /* will adjust below */
        /* Compute total sum of a[0]..a[27] first */
        seq_t total_sum = cache[0];
        for (idx_t i = 1; i <= 27; ++i)
            total_sum += cache[i];
        sum_last27 = total_sum - cache[0];                /* sum a[1]..a[27] */

        /* a(28) = b_28 + 28*sum_last27 - 406*a(0)  with b_28 = 1 */
        seq_t term28 = (seq_t)1 + mul_small(sum_last27, 28) - mul_small(cache[0], 406);
        cache[28] = term28;
        cache_size = 29;
    }

    /* If n is exactly 28, we are done */
    if (n < cache_size)
        return cache[n];

    /* -----------------------------------------------------------------
       Sliding window for n ≥ 29:
       For each i from 29 up to n:
         a(i) = 28*sum_{k=i-27}^{i-1} a(k) - 406*a(i-28)
       The array cache already contains a[0]..a[28] at this point.
       ----------------------------------------------------------------- */

    /* First prepare the sum of a[2]..a[28] (27 terms) for i = 29 */
    seq_t sum_last27 = cache[1];          /* will accumulate below */
    for (idx_t k = 2; k <= 28; ++k)
        sum_last27 += cache[k];

    /* Now compute a[29] .. a[n] */
    for (idx_t i = 29; i <= n; ++i) {
        seq_t term = mul_small(sum_last27, 28) - mul_small(cache[i - 28], 406);
        cache[i] = term;

        /* Update the sliding window sum for the next iteration:
           new sum = old sum + a(i) - a(i-27)   */
        sum_last27 += term;
        sum_last27 -= cache[i - 27];
    }

    cache_size = n + 1;   /* we have computed up to index n */
    return cache[n];
}
