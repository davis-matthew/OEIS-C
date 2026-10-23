/*
 * A170598  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=13, ..., and for n>48 a(n) =
 *            11*(a(n-1)+a(n-2)+…+a(n-47)) - 66*a(n-48)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached terms a(0)…a(k) */
    static idx_t cache_size  = 0;      /* number of stored terms          */
    static size_t capacity   = 0;      /* allocated size of cache         */

    /* Ensure the cache can hold up to a(n). */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache    = tmp;
        capacity = new_cap;
    }

    if (n < cache_size) return cache[n];

    /* Compute the first 49 terms (0 … 48). */
    if (cache_size <= 48) {
        size_t need = 49;
        if (capacity < need) {
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!tmp) abort();
            cache    = tmp;
            capacity = need;
        }

        for (idx_t m = 0; m < 49; ++m) {
            seq_t sum = 0;
            idx_t up = (m < 47) ? m : 47;          /* how many previous terms to sum */
            for (idx_t i = 1; i <= up; ++i)
                sum += cache[m - i];                /* sum_{i=1}^{up} a(m-i) */

            seq_t num = (m == 0 || m == 48) ? (seq_t)1 : (seq_t)2;
            seq_t val = num + (seq_t)11 * sum;      /* a(m) = num + 11*sum */
            if (m >= 48)
                val -= (seq_t)66 * cache[m - 48];   /* minus 66*a(m-48) for m≥48 */

            cache[m] = val;
        }
        cache_size = 49;                        /* we now have a(0)…a(48) */
    }

    /* Compute subsequent terms up to a(n). */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= 47; ++i)
            sum += cache[m - i];                /* sum_{i=1}^{47} a(m-i) */

        seq_t val = (seq_t)11 * sum - (seq_t)66 * cache[m - 48];
        cache[m] = val;
    }

    cache_size = n + 1;                         /* update stored range   */
    return cache[n];
}
