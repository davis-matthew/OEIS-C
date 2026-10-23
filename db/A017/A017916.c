/*
 * A017916  Expansion of 1/((1-3x)*(1-5x)*(1-10x)).
 * https://oeis.org/A017916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 95*a(n-2) + 150*a(n-3), a(0) = 1, a(1) = 18,
 *            a(2) = 229 (OEIS A017916)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)229;

    /* Static cache to store computed terms for any order of calls. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t len = 0;   /* number of terms already computed */

    /* Initialize cache with first three terms. */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)229;
        len   = 3;
    }

    /* Ensure capacity for n+1 entries. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 3;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)18 * cache[i-1] - (seq_t)95 * cache[i-2] + (seq_t)150 * cache[i-3];
    }
    len = (size_t)n + 1;   /* all terms up to n are now computed */

    return cache[n];
}
