/*
 * A142982  a(1) = 1, a(2) = 9, a(n+2) = 9*a(n+1) + (n + 1)^2*a(n).
 * https://oeis.org/A142982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 9, a(n+2) = 9*a(n+1) + (n+1)^2*a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A142982 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization cache (1‑indexed) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated length of cache array */
    static idx_t last = 0;         /* highest index computed so far */

    if (n == 0)      /* undefined for n<1; return 0 as a safe fallback */
        return (seq_t)0;

    /* Initial allocation */
    if (cache == NULL) {
        cache_len = 3;                      /* indices 0..2; 0 unused */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)0;                /* dummy value for index 0 */
        cache[1] = (seq_t)1;                /* a(1) */
        cache[2] = (seq_t)9;                /* a(2) */
        last = 2;
    }

    /* If requested n already computed, return it */
    if (n <= last)
        return cache[n];

    /* Grow cache if necessary */
    if (cache_len <= n) {
        size_t new_len = cache_len;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = 9*a(i-1) + (i-1)^2 * a(i-2)   (since i = n+2) */
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t square = (seq_t)(i - 1) * (seq_t)(i - 1);
        seq_t term = (seq_t)9 * prev1 + square * prev2;
        cache[i] = term;
    }

    last = n;
    return cache[n];
}
