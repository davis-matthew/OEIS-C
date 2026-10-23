/*
 * A041701  Denominators of continued fraction convergents to sqrt(370).
 * https://oeis.org/A041701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 654*a(n-3) + a(n-6)  for n > 5
 * Initial values: 
 *   a(0)=1, a(1)=4, a(2)=17, a(3)=650, a(4)=2617, a(5)=11118
 * The function caches previously computed values to allow arbitrary order
 * of calls while remaining O(n) overall.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS A041701 */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;          /* Highest index computed */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* Allocate space for the first 6 terms */
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)650;
        cache[4] = (seq_t)2617;
        cache[5] = (seq_t)11118;
        max_computed = 5;
    }

    /* Extend cache to hold up to n */
    if (n > max_computed) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i) = 654*a(i-3) + a(i-6) */
            cache[i] = (seq_t)654 * cache[i - 3] + cache[i - 6];
        }
        max_computed = n;
    }

    return cache[n];
}
