/*
 * A011367  Expansion of (1-x^2-x^3)/(1-2*x-5*x^2-4*x^3-x^4).
 * https://oeis.org/A011367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) + 4*a(n-3) + a(n-4) (n >= 4) with
 *            a(0) = 1, a(1) = 2, a(2) = 8, a(3) = 29
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current allocated size (>=1) */

    /* initialise base terms once */
    if (cache == NULL) {
        cap = 4;                    /* store a(0)..a(3) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) {              /* memory allocation failed */
            exit(EXIT_FAILURE);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)29;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = cap > 4 ? cap - cap : 4; i <= (size_t)n; ++i) {
        /* guard against reading uninitialised elements when i < 4 */
        if (i < 4) continue;
        seq_t val = cache[i-1] * (seq_t)2;
        val += cache[i-2] * (seq_t)5;
        val += cache[i-3] * (seq_t)4;
        val += cache[i-4];
        cache[i] = val;
    }

    return cache[(size_t)n];
}
