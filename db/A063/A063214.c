/*
 * A063214  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          46 ).
 * https://oeis.org/A063214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=6, a(3)=10, a(4)=12, a(5)=16, a(6)=22 a(n) = a(n-2)
 *            + a(n-3) – a(n-5) for n > 6
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recurrence implementation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* 1‑indexed cache */
    static idx_t cache_cap = 0;          /* current capacity (size) */

    /* Ensure the cache can hold index n and the first 6 terms */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, return 0 as safe value */
        cache = tmp;
        /* Initialize newly allocated entries up to new_cap-1 with zeros */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Fill base cases if not already filled */
    if (cache[1] == 0) {          /* not initialized yet */
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 10;
        cache[4] = 12;
        cache[5] = 16;
        cache[6] = 22;
    }

    /* If requested term already computed, return it */
    if (cache[n] != 0) return cache[n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t s = cache[i - 2] + cache[i - 3];
        cache[i] = s - cache[i - 5];
    }

    return cache[n];
}
