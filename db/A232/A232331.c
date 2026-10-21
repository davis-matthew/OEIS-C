/*
 * A232331  Number of nX4 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally, with no adjacent
 *          elements equal.
 * https://oeis.org/A232331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) - 8*a(n-3) - 4*a(n-4); initial
 *            values: a(1)=6, a(2)=32, a(3)=154, a(4)=734
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation of the recurrence */
    static seq_t *cache = NULL;   /* allocated slots 0..cache_len-1 (use 1..) */
    static idx_t cache_len = 0;   /* current allocation size (>=1 if allocated) */
    static idx_t computed = 0;    /* highest index that has been computed */

    /* Off‑by‑one: the sequence is defined for n >= 1; index 0 is unused. */
    if (n == 0)
        return (seq_t)0;   /* undefined but harmless */

    /* Initialise the first few terms when the cache is empty */
    if (computed == 0) {
        /* Allocate room for indices 0..4 (index 0 unused) */
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* out of memory – abort silently */

        cache[1] = (seq_t)6;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)154;
        cache[4] = (seq_t)734;
        computed = 4;
    }

    /* Return a cached value if available */
    if (n <= computed)
        return cache[n];

    /* Grow the array if necessary (never shrinking) */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory – abort silently */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute terms from the first missing index up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)7 * cache[i - 1];
        term -= (seq_t)9 * cache[i - 2];
        term -= (seq_t)8 * cache[i - 3];
        term -= (seq_t)4 * cache[i - 4];
        cache[i] = term;
    }
    computed = n;

    return cache[n];
}
