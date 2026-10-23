/*
 * A167078  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            23*(a(n-1)+...+a(n-12)) – 276*a(n-13) + b(n) where b(0)=1,
 *            b(1..12)=2, b(13)=1, and b(n)=0 for n>13. a(0)=1, a(1)=25,
 *            a(2)=600, … (obtained from the recurrence)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n) {
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* size of allocated buffer */
    static size_t cache_size = 0;  /* number of values actually stored */

    /* Ensure we have space for index n */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache_size = 0;
    }
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    while (cache_size <= n) {
        idx_t k = cache_size;                 /* index to compute */
        seq_t rhs;                            /* b(k) term */
        if (k == 0 || k == 13) rhs = (seq_t)1;
        else if (k <= 12) rhs = (seq_t)2;
        else rhs = (seq_t)0;                  /* for k > 13 */

        seq_t sum = (seq_t)0;
        /* sum 23 * a(k-i) for i = 1 .. min(k,12) */
        idx_t h = (k <= 12 ? k : 12);
        for (idx_t i = 1; i <= h; ++i)
            sum += (seq_t)23 * cache[k - i];

        /* subtract 276 * a(k-13) if k >= 13 */
        if (k >= 13)
            sum -= (seq_t)276 * cache[k - 13];

        seq_t val = sum + rhs;
        cache[k] = val;
        ++cache_size;
    }

    return cache[n];
}
