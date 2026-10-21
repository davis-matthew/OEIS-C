/*
 * A233152  Number of n X 5 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally or antidiagonally.
 * https://oeis.org/A233152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233152: Number of n×5 0..2 arrays with no element x(i,j) adjacent
 * to value 2−x(i,j) horizontally or antidiagonally.
 *
 * Recurrence:  a(1) = 48,  a(2) = 648
 *               a(n) = 15*a(n-1) – 18*a(n-2)   for n ≥ 3
 */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoisation – 1‑based indexing, index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of entries allocated */

    /* Guard for calls with n == 0 (outside the OEIS offset) */
    if (n == 0)  return 0;

    /* Ensure cache is large enough to hold indices up to n */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;   /* to access cache[n] */

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {       /* allocation failure – return 0 */
            return 0;
        }
        cache = tmp;
        cache_len = new_len;

        /* Zero‑initialise new part of the cache */
        for (idx_t i = old_len; i < new_len; ++i) {
            cache[i] = (seq_t)0;
        }
    }

    /* Initialise the known seed values once */
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)48;
    if (cache_len > 2 && cache[2] == (seq_t)0) cache[2] = (seq_t)648;

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            cache[i] = (seq_t)(15 * cache[i-1] - 18 * cache[i-2]);
        }
    }

    return cache[n];
}
