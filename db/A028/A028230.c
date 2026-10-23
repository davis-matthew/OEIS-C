/*
 * A028230  Bisection of A001353. Indices of square numbers which are also
 *          octagonal.
 * https://oeis.org/A028230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 14*a(n-1) - a(n-2) with a(1)=1, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function maintains a static cache so repeated calls are O(1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined case, but safe */

    /* static cache and its current size (number of elements stored) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* elements stored correspond to indices 1..cache_size */

    if (n > cache_size) {
        /* Allocate / resize cache to hold up to n elements */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed */
            exit(1);
        }
        cache = tmp;

        /* Compute new terms, starting from the first missing index */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;        /* a(1) */
            } else if (i == 1) {
                cache[i] = (seq_t)15;       /* a(2) */
            } else {
                cache[i] = (seq_t)14 * cache[i-1] - cache[i-2];
            }
        }
        cache_size = n;
    }

    return cache[n-1];
}
