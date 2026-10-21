/*
 * A204089  The number of 1 by n Haunted Mirror Maze puzzles with a unique
 *          solution ending with a mirror, where mirror orientation is fixed.
 * https://oeis.org/A204089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) (n >= 3), a(0) = 1, a(1) = 1, a(2) =
 *            4 (This recurrence gives the OEIS sequence A204089.)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
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
    static idx_t max_cached = 0;        /* largest n with a value stored */

    /* initialise cache for n=0,1,2 on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..2 initially */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache[2] = (seq_t)4;   /* a(2) = 4 */
        max_cached = 2;
    }

    /* grow cache if needed and compute missing terms */
    if (n > max_cached) {
        /* reallocate to hold indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;      /* out‑of‑memory: return 0 */
        cache = new_cache;
        /* compute terms from max_cached+1 upto n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)4 - cache[i - 2] * (seq_t)2;
        }
        max_cached = n;
    }

    return cache[n];
}
