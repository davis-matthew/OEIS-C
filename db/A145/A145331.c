/*
 * A145331  Numbers X such that there exists Y in N : X^2=129*Y^2+43.
 * https://oeis.org/A145331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 33710*a(n+1) - a(n), a(1) = 602, a(2) = 20292818
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t cached_max = 0;

    /* ensure enough capacity */
    if (n > cap) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise first values if necessary */
    if (cached_max < 1) {
        cache[0] = (seq_t)602;
        cached_max = 1;
    }
    if (cached_max < 2) {
        cache[1] = (seq_t)20292818;
        cached_max = 2;
    }

    /* extend cache up to n */
    while (cached_max < n) {
        /* a(i+1) = 33710*a(i) - a(i-1)  (1‑based indices) */
        seq_t a_prev1 = cache[cached_max - 1];     /* a(n)   */
        seq_t a_prev2 = cache[cached_max - 2];     /* a(n-1) */
        seq_t val = (seq_t)33710 * a_prev1 - a_prev2;
        cache[cached_max] = val;
        ++cached_max;
    }

    return cache[n - 1];
}
