/*
 * A009923  Coordination sequence for CaF2(1), Ca position.
 * https://oeis.org/A009923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-2) + 2*a(n-3) -
 *            2*a(n-5) - a(n-6) + a(n-8) for n >= 10 with initial terms
 *            a(0)=1, a(1)=8, a(2)=36, a(3)=78, a(4)=140, a(5)=230, a(6)=308,
 *            a(7)=438, a(8)=572, a(9)=710
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {1, 8, 36, 78, 140, 230, 308, 438, 572, 710};

    /* Fast path for small indices. */
    if (n < 10)
        return init[n];

    /* Cache to store previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* Highest index computed so far. */
    static size_t cap = 0;       /* Allocated capacity of cache. */

    /* Initialize cache on first use. */
    if (!cache) {
        cap = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return 0; /* Allocation failure; caller will not use result. */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        cached_n = 9;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cached_n)
        return cache[n];

    /* Ensure enough capacity. */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            return 0; /* Reallocation failure. */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* a(i) = a(i-2) + 2*a(i-3) - 2*a(i-5) - a(i-6) + a(i-8) */
        seq_t pos_sum = cache[i - 2];
        pos_sum += cache[i - 3];
        pos_sum += cache[i - 3];
        pos_sum += cache[i - 8];

        seq_t neg_sum = cache[i - 5];
        neg_sum += cache[i - 5];
        neg_sum += cache[i - 6];

        cache[i] = pos_sum - neg_sum;
    }

    cached_n = n;
    return cache[n];
}
