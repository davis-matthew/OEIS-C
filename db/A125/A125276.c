/*
 * A125276  Eigensequence of triangle A039598: a(n) = Sum_{k=0..n-1}
 *          A039598(n-1,k)*a(k) for n>0 with a(0)=1.
 * https://oeis.org/A125276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} a(k) * C(2*n,n-k-1)*(k+1)/n, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binom(idx_t a, idx_t b) from the provided utils */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* static cache to store all computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;  /* allocated size */
    static idx_t cache_len = 0;  /* number of valid entries (<= cache_cap) */

    /* Ensure the cache is large enough */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return 0 */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Fill missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t t = binom(2 * i, i - k - 1);   /* C(2*i, i-k-1) */
            t = t * (k + 1);                    /* multiply (k+1) */
            t = t / i;                          /* divide by i (integer division) */
            sum += t * cache[k];                /* accumulate */
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
