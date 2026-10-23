/*
 * A192240  Constant term in the reduction of the polynomial (x+3)^n by x^2 ->
 *          x+1.
 * https://oeis.org/A192240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3; for n≥2, a(n)=7*a(n-1)-11*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of computed a(i) values   */
    static size_t cap = 0;        /* allocated capacity of cache      */
    static idx_t maxComputed = 1; /* highest index that has been filled */

    /* Allocate or grow the cache to hold a(n) */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)3;
    } else if ((size_t)n + 1 > cap) {
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (cap > 1) cache[1] = (seq_t)3;
    }

    /* Handle base cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Compute any missing entries up to n */
    if (maxComputed < (idx_t)n) {
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            cache[i] = (cache[i - 1] * (seq_t)7) - (cache[i - 2] * (seq_t)11);
        }
        maxComputed = n;
    }
    return cache[n];
}
