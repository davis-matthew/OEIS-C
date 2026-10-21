/*
 * A219454  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and columns, 0..3 n X 2 array.
 * https://oeis.org/A219454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            14. Initial values (n = 1..14):
 *            4,4,13,26,57,116,231,450,859,1604,2928,5228,9139,15653
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 62, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t init[15] = {
    0ULL,      /* unused 0 */
    4ULL, 4ULL, 13ULL, 26ULL, 57ULL, 116ULL, 231ULL, 450ULL,
    859ULL, 1604ULL, 2928ULL, 5228ULL, 9139ULL, 15653ULL
};

static seq_t *cache = NULL;   /* array indexed from 0, cache[0] unused */
static idx_t  cache_last = 0ULL;   /* highest n for which cache[n] computed */
static idx_t  cache_size = 0ULL;   /* allocated size (cache_size == 0 -> NULL) */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return init[0];   /* safety; sequence is undefined for n=0 */

    /* Direct return for first 14 values */
    if (n <= 14ULL)
        return init[(size_t)n];

    /* Allocate or resize cache if necessary */
    if (cache_size < n + 1ULL) {
        idx_t new_size = n + 1ULL;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();                /* out‑of‑memory */
        cache = tmp;
        cache_size = new_size;

        /* Ensure the initial 14 terms are present */
        for (idx_t i = 1; i <= 14ULL && i < cache_size; ++i)
            cache[i] = init[(size_t)i];
    }

    /* Compute terms beyond those already cached */
    for (idx_t i = cache_last + 1ULL; i <= n; ++i) {
        if (i <= 14ULL) {  /* already initialized above */
            cache[i] = init[(size_t)i];
            continue;
        }

        seq_t val =
            9ULL       * cache[i-1ULL]
          - 36ULL      * cache[i-2ULL]
          + 84ULL      * cache[i-3ULL]
          - 126ULL     * cache[i-4ULL]
          + 126ULL     * cache[i-5ULL]
          - 84ULL      * cache[i-6ULL]
          + 36ULL      * cache[i-7ULL]
          - 9ULL       * cache[i-8ULL]
          + cache[i-9ULL];

        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
