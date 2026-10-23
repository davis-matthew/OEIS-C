/*
 * A163608  a(n) = ((5 + 2*sqrt(2))*(2 + sqrt(2))^n + (5 - 2*sqrt(2))*(2 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2); a(0) = 5, a(1) = 14
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: dynamically grow only as needed */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static size_t capacity = 0;          /* allocated size (slots)    */
    static size_t used = 0;              /* number of initialized terms */

    /* Initialisation on first call */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)14;
        used = 2;
    }

    /* If the requested index is already known, return it */
    if (n < used) {
        return cache[(size_t)n];
    }

    /* Grow the cache until it can hold index n */
    while (capacity <= (size_t)n) {
        capacity = capacity ? capacity * 2 : 2;
    }
    seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = used; i <= (size_t)n; ++i) {
        seq_t term1 = cache[i - 1] * (seq_t)4;
        seq_t term2 = cache[i - 2] * (seq_t)2;
        cache[i] = term1 - term2;
    }

    used = (size_t)n + 1;
    return cache[(size_t)n];
}
