/*
 * A203980  Number of (n+1) X 5 0..2 arrays with no 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A203980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 270*a(n-3) + 324*a(n-4) Initial values:
 *            a(1)=384, a(2)=5184, a(3)=69984, a(4)=956448
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static idx_t cached_to = 0;     /* highest index computed */

    if (n < 1)
        return (seq_t)0;           /* sequence undefined for n < 1 */

    /* Allocate or grow cache to fit up to index n */
    if (cache == NULL || cache_size < (size_t)(n + 5)) {
        size_t new_size = (size_t)(n + 5);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize base terms once */
    if (cached_to < 1) {
        cache[1] = (seq_t)384;
        cached_to = 1;
    }
    if (cached_to < 2) {
        cache[2] = (seq_t)5184;
        cached_to = 2;
    }
    if (cached_to < 3) {
        cache[3] = (seq_t)69984;
        cached_to = 3;
    }
    if (cached_to < 4) {
        cache[4] = (seq_t)956448;
        cached_to = 4;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_to + 1; i <= n; ++i) {
        seq_t term = (seq_t)15 * cache[i - 1];
        term = term - (seq_t)270 * cache[i - 3];
        term = term + (seq_t)324 * cache[i - 4];
        cache[i] = term;
    }
    cached_to = n;
    return cache[n];
}
