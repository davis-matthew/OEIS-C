/*
 * A016263  Expansion of 1/((1-x)(1-9x)(1-12x)).
 * https://oeis.org/A016263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 108*a(n-2) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial conditions: a(0) = 1, a(1) = 22           */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t cached = 0;     /* number of values stored */

    /* ensure the array is large enough */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
    } else if (need > cached) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    }

    /* initialise the memoised entries that are missing */
    if (cached == 0) {
        cache[0] = (seq_t)1;          /* a(0) */
        cached = 1;
    }
    if (cached == 1 && n >= 1) {
        cache[1] = (seq_t)22;         /* a(1) */
        cached = 2;
    }

    /* compute values up to n using the recurrence */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        /* avoid unsigned underflow: compute 21*a[i-1] + 1 first */
        seq_t temp = (seq_t)21 * cache[i - 1] + (seq_t)1;
        cache[i] = temp - (seq_t)108 * cache[i - 2];
    }
    cached = (size_t)n + 1;

    return cache[(size_t)n];
}
