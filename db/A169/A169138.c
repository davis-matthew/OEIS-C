/*
 * A169138  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*(a(n-1)+a(n-2)+…+a(n-25)) – 378*a(n-26) + b(n) where
 *            b(n) = 1 for n=0,26; b(n)=2 for 1≤n≤25; b(n)=0 otherwise. The
 *            recurrence is applied with the convention a(k)=0 for k<0. The
 *            function caches computed values for arbitrary‑order calls
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached sequence values         */
    static size_t cache_len = 0;    /* number of values currently stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache       */

    /* Expand cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t term;
        seq_t sum = 0;
        /* sum of last 25 values */
        for (size_t k = 1; k <= 25 && i >= k; ++k)
            sum += cache[i - k];

        /* recurrence part */
        term = sum * (seq_t)27;
        if (i >= 26)
            term -= cache[i - 26] * (seq_t)378;

        /* b(i) part */
        seq_t b;
        if (i == 0 || i == 26)
            b = (seq_t)1;
        else if (i <= 25)
            b = (seq_t)2;
        else
            b = (seq_t)0;

        term += b;
        cache[i] = term;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
