/*
 * A169428  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) - 29*sum_{k=1}^{31} a(n-k) + 435*a(n-32) = b(n) with
 *            b(0)=1, b(1..31)=2, b(32)=1, b(n)=0 otherwise and a(i)=0 for
 *            i<0. This is the linear recurrence implied by the generating
 *            function (1+2∑_{k=1}^{31}x^k+x^32)/(1-29∑_{k=1}^{31}x^k+435x^32)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* capacity of cache */

    /* expand cache up to n if necessary */
    if (cache_len <= n) {
        /* increase capacity as needed */
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* sum of last 31 terms */
            seq_t sum32 = 0;
            for (idx_t k = 1; k <= 31; ++k) {
                if (i >= k) sum32 += cache[i - k];
            }
            /* base term b(i) */
            seq_t b = 0;
            if (i == 0) b = (seq_t)1;
            else if (i <= 31) b = (seq_t)2;
            else if (i == 32) b = (seq_t)1;

            /* recurrence value */
            seq_t val = b + (seq_t)29 * sum32;
            if (i >= 32) val -= (seq_t)435 * cache[i - 32];
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
