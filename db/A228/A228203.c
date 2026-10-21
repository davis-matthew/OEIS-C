/*
 * A228203  x-values in the solution to x^2 - 13y^2 = 27.
 * https://oeis.org/A228203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A228203 – x-values in the solution to x^2 - 13y^2 = 27
 *  a(n) = 1298*a(n-4) - a(n-8)
 *  initial values (a(1) .. a(8)):
 *     12, 40, 220, 768, 14808, 51700, 285520, 996852
 */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Cache of computed terms.
     *  cache[i] holds a(i+1)   (array index 0 corresponds to a(1)) */
    static seq_t *cache = NULL;
    static size_t capacity = 0;       /* current allocated array size (in elements) */
    static idx_t maxComputed = 0;     /* highest n that has been computed and cached */

    /* --- initialise cache on first call --------------------------------- */
    if (!cache) {
        /* Ensure room for at least the first 8 terms. */
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();

        /* store the first 8 known terms */
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)220;
        cache[3] = (seq_t)768;
        cache[4] = (seq_t)14808;
        cache[5] = (seq_t)51700;
        cache[6] = (seq_t)285520;
        cache[7] = (seq_t)996852;
        maxComputed = 8;
    }

    /* --- grow the cache if required ------------------------------------- */
    if ((size_t)n > capacity) {
        size_t newCap = capacity;
        if (newCap == 0) newCap = 8;
        while (newCap < (size_t)n) newCap <<= 1;   /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = newCap;
    }

    /* --- return cached value if already computed ------------------------ */
    if (n <= maxComputed) {
        return cache[n - 1];
    }

    /* --- compute missing terms using the recurrence -------------------- */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        /* array index for a(i) : i-1 */
        size_t idx      = (size_t)(i - 1);
        size_t idx_a4   = (size_t)(i - 5);   /* a(i-4) -> cache[i-5] */
        size_t idx_a8   = (size_t)(i - 9);   /* a(i-8) -> cache[i-9] */

        seq_t val = cache[idx_a4] * (seq_t)1298;
        val -= cache[idx_a8];
        cache[idx] = val;
    }
    maxComputed = n;

    return cache[n - 1];
}
