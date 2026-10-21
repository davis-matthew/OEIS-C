/*
 * A233078  Number of n X 4 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (49 + 27·49ⁿ)/98, equivalently a(1)=14, a(2)=662, and for
 *            n≥3 a(n)=50·a(n‑1) – 49·a(n‑2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external integer types are provided by the build system */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values, 1‑based */
    static size_t cache_size = 0;
    static idx_t cached_up_to = 0; /* highest n already computed */

    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 – safety */
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)662;

    /* initialise cache on first use */
    if (!cache) {
        cache_size = 4; /* indices 0..3 are safe */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) exit(1);              /* allocation failure */
        cache[0] = (seq_t)0;              /* unused */
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)662;
        cached_up_to = 2;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size;
        while ((size_t)n >= new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)(50 * prev1 - 49 * prev2);
    }
    cached_up_to = n;

    return cache[n];
}
