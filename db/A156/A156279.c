/*
 * A156279  4 times the Lucas number A000032(n).
 * https://oeis.org/A156279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*A000032(n) Recurrence: a(0)=8, a(1)=4,
 *            a(n)=a(n-1)+a(n-2) for n≥2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t computed = 0;    /* number of values currently stored */
    static size_t cap = 0;         /* allocated array capacity */

    /* On the first call initialise the cache with the two seed values. */
    if (computed == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* memory failure – return zero as a fallback */
        cache[0] = (seq_t)8;       /* a(0) */
        cache[1] = (seq_t)4;       /* a(1) */
        computed = 2;
    }

    /* If the requested term is already cached, return it immediately. */
    if ((size_t)n < computed)
        return cache[n];

    /* Determine how many terms we must compute. */
    size_t needed = (size_t)n + 1;

    /* Grow the cache if necessary. */
    if (cap < needed) {
        size_t newcap = (cap == 0) ? 2 : cap;
        while (newcap < needed)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* memory failure – return zero as a fallback */
        cache = tmp;
        cap = newcap;
    }

    /* Compute the missing terms using the recurrence. */
    for (size_t i = computed; i < needed; ++i)
        cache[i] = cache[i - 1] + cache[i - 2];

    computed = needed;
    return cache[n];
}
