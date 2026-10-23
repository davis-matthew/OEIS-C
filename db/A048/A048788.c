/*
 * A048788  a(2n+1) = a(2n) + a(2n-1), a(2n) = 2*a(2n-1) + a(2n-2); a(n) = n
 *          for n = 0, 1.
 * https://oeis.org/A048788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 4*a(n-2) - a(n-4); a(0)=0, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* Initialize cache with the first four terms */
    if (!cache) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        max_n  = 3;
    }

    /* Return cached value if already computed */
    if (n <= max_n)
        return cache[n];

    /* Ensure the cache array is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > max_n + 1) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed, return 0 as safe fallback */
        cache = tmp;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i < 4)
            continue; /* initial terms already set */
        cache[i] = 4 * cache[i - 2] - cache[i - 4];
    }

    max_n = n;
    return cache[n];
}
