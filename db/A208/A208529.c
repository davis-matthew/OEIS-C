/*
 * A208529  Number of permutations of n > 1 having exactly 2 points on the
 *          boundary of their bounding square.
 * https://oeis.org/A208529
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*(n-2), a(2) = 2 (OEIS A208529)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* We rely on the build providing the types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* For safety, return 0 for indices before the offset. */
    if (n < 2)
        return (seq_t)0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) for i >= 2 */
    static idx_t last_n = 1;      /* highest n computed so far */
    static size_t cap = 0;        /* allocated cache size */

    /* Resize cache if needed */
    if (n >= cap) {
        size_t new_cap = (cap == 0 ? (size_t)(n + 1) : cap * 2);
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)     /* out of memory: abort */
            abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            /* a(i) = a(i-1) * (i-2) */
            cache[i] = cache[i - 1] * (seq_t)(i - 2);
        }
    }
    last_n = n;

    return cache[n];
}
