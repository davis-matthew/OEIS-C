/*
 * A155450  Numbers congruent to 5 or 18 mod 23.
 * https://oeis.org/A155450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3) with initial values a(1)=5, a(2)=18,
 *            a(3)=28
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n == 0)
        return 0;            /* undefined for n=0, harmless */

    /* Ensure the cache has enough space for indices 1..n (zero‑based. */
    if ((idx_t)cap < n) {
        idx_t new_cap = n;
        seq_t *new_cache =
            (seq_t *)malloc(sizeof(seq_t) * (size_t)new_cap);
        if (cache) {
            for (idx_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        for (idx_t i = cap; i < new_cap; ++i)
            new_cache[i] = 0;   /* 0 is a safe sentinel (all terms >0) */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any uncomputed terms up to n, in order. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t *p = &cache[i - 1];
        if (*p != 0) /* already computed */
            continue;

        if (i == 1)
            *p = 5;
        else if (i == 2)
            *p = 18;
        else if (i == 3)
            *p = 28;
        else
            *p = cache[i - 2] + cache[i - 3] - cache[i - 4];
    }

    return cache[n - 1];
}
