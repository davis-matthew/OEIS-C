/*
 * A169251  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence derived from the rational g.f. a(n) = 44*a(n-1) +
 *            44*a(n-2) + … + 44*a(n-27) – 990*a(n-28) for n >= 28; a(0)=1;
 *            for 1 <= n <= 27, a(n)=44*∑_{k=0}^{n-1}a(k)+2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values of a(0)..a(m) */
    static idx_t  cache_len = 0;    /* number of entries stored in cache */

    static seq_t cumulative_sum = 0; /* Σ a[0..m-1] for the last computed m   */
    static seq_t sum27 = 0;          /* Σ a[m-27..m-1] (last 27 terms)       */

    /* expand cache if required */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        /* cast of realloc for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;

        for (idx_t m = old_len; m <= n; ++m) {
            if (m == 0) {                     /* a(0) = 1 */
                cache[0] = (seq_t)1;
                cumulative_sum = (seq_t)1;
                continue;
            }

            if (m <= 27) {                    /* 1 <= m <= 27 */
                seq_t val = (seq_t)44 * cumulative_sum + (seq_t)2;
                cache[m] = val;
                cumulative_sum += val;
                if (m == 27)
                    sum27 = cumulative_sum - cache[0]; /* Σ a[1..27] */
                continue;
            }

            /* recurrence for m >= 28 */
            seq_t val = (seq_t)44 * sum27 - (seq_t)990 * cache[m - 28];
            cache[m] = val;
            cumulative_sum += val;

            /* update sliding window sum27:
               new sum27 = previous sum27 + a(m) – a(m-27) */
            sum27 += val;
            sum27 -= cache[m - 27];
        }
    }

    return cache[n];
}
