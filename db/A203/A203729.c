/*
 * A203729  Number of (n+1) X 3 0..7 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n‑1) + 238*a(n‑2) for n ≥ 3 Initial values: a(1) =
 *            14400, a(2) = 539456
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache for memoization: 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* we allocate indices 0…cache_size-1 */
    static idx_t computed = 0;     /* highest index computed so far */

    if (n == 0) return (seq_t)0;   /* offset starts at 1; unused */

    /* Ensure cache has room for index n */
    if (n >= cache_size) {
        idx_t new_size = cache_size > 0 ? cache_size : 1;
        while (new_size <= n) new_size <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            /* In a contest environment one might exit or handle error */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize base cases if needed */
    if (computed == 0 && n >= 1) {
        cache[1] = (seq_t)14400;
        computed = 1;
    }
    if (computed < 2 && n >= 2) {
        cache[2] = (seq_t)539456;
        computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)29 * cache[i - 1] + (seq_t)238 * cache[i - 2];
    }
    computed = n;
    return cache[n];
}
