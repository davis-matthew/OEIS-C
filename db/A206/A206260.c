/*
 * A206260  Number of (n+1) X 2 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n>6,
 *            a(1)=16, a(2)=24, a(3)=34, a(4)=50, a(5)=72, a(6)=105
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms, 1‑based indexing */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots */
    static idx_t computed   = 0;   /* highest n that has been computed */

    if (n == 0) return 0;          /* offset is 1 */

    /* Ensure the cache has at least n slots. */
    if (cache_size < n) {
        /* realloc returns void*, so cast explicitly for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failed */
        cache     = tmp;
        cache_size = n;
    }

    /* Initialise the first six terms once. */
    if (computed < 6) {
        idx_t up = (n < 6) ? n : 6;
        if (up >= 1) cache[0] = 16;
        if (up >= 2) cache[1] = 24;
        if (up >= 3) cache[2] = 34;
        if (up >= 4) cache[3] = 50;
        if (up >= 5) cache[4] = 72;
        if (up >= 6) cache[5] = 105;
        computed = up;
    }

    /* Compute any remaining terms up to n. */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* i>6 ensures all needed predecessors are present */
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a2 = cache[i - 3];   /* a(i-2) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            seq_t a4 = cache[i - 5];   /* a(i-4) */
            seq_t a5 = cache[i - 6];   /* a(i-5) */
            cache[i - 1] = 4 * a1 - 5 * a2 + a3 + 2 * a4 - a5;
        }
        computed = n;
    }

    return cache[n - 1];
}
