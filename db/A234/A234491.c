/*
 * A234491  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 74*a(n-2) - 295*a(n-3) - 1008*a(n-4) +
 *            840*a(n-5) Initial values (offset 1): a(1)=250, a(2)=1834,
 *            a(3)=13464, a(4)=99142, a(5)=728536
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined below offset */

    static seq_t *cache = NULL;
    static size_t size = 0;        /* largest computed index */
    static size_t cap  = 0;        /* allocated capacity */

    /* initialise cache on first call */
    if (!cache) {
        cap = 6;                  /* indices 0..5, 0 unused */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)250;
        cache[2] = (seq_t)1834;
        cache[3] = (seq_t)13464;
        cache[4] = (seq_t)99142;
        cache[5] = (seq_t)728536;
        size   = 5;
    }

    /* return cached value if already computed */
    if ((size_t)n <= size)
        return cache[(size_t)n];

    /* compute necessary values up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        /* extend array if needed */
        while ((size_t)i >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* recurrence, computed as (positive terms) – (negative terms) */
        seq_t pos = cache[i - 1] * (seq_t)5
                  + cache[i - 2] * (seq_t)74
                  + cache[i - 5] * (seq_t)840;
        seq_t neg = cache[i - 3] * (seq_t)295
                  + cache[i - 4] * (seq_t)1008;

        cache[i] = pos - neg;      /* pos > neg, so no underflow for unsigned types */
    }

    size = (size_t)n;
    return cache[(size_t)n];
}
