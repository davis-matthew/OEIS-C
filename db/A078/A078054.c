/*
 * A078054  Expansion of (1-x)/(1+2*x-2*x^2+x^3).
 * https://oeis.org/A078054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1)+2*a(n-2)-a(n-3), a(0)=1, a(1)=-3, a(2)=8
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values for recurrence */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t computed = 0;     /* next index to compute (cache[0..computed-1] valid) */

    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)8;

    /* Initialise cache with the known base cases */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)8;
        computed = 3;                          /* all 0..2 are now available */
    }

    /* Expand cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = (seq_t)-2 * cache[i-1] + (seq_t)2 * cache[i-2] - cache[i-3];
    }
    computed = n + 1;

    return cache[n];
}
