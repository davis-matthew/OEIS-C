/*
 * A137565  Hankel transform of quadruple factorial numbers A001813.
 * https://oeis.org/A137565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏{k=0..n-1} (8*(k+1)*(2k+1))^(n-k) (n ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1,  a(1) = 8,  a(2) = 3072,  a(3) = 141557760, ... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache   = NULL;    /* array of a(i) */
    static size_t cache_len = 0;     /* number of cached terms (always ≥1 after init) */
    static seq_t prod_b = 1;         /* ∏_{i=1}^{current} 8*i*(2*i-1) for the last computed index */

    /* initialise cache for a(0) */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = 1;          /* a(0) */
        prod_b  = 1;
    }

    /* extend cache up to the required index n */
    while (cache_len <= n) {
        idx_t idx = (idx_t)cache_len;               /* next index to compute */
        seq_t term = (seq_t)8 * (seq_t)idx * (seq_t)(2 * idx - 1);
        prod_b *= term;                 /* update product of terms up to idx */
        seq_t next_val = cache[idx - 1] * prod_b;  /* a(idx) = a(idx-1) * prod_b */

        /* grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache[cache_len] = next_val;
        cache_len++;
    }

    return cache[n];
}
