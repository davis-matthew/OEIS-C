/*
 * A145320  Numbers n such that there exists x in N : (x+31)^3-x^3=n^2.
 * https://oeis.org/A145320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145320: Numbers n such that there exists x∈ℕ with (x+31)^3‑x^3=n².
   a(1)=6727, a(2)=163472827, a(n+2)=24302·a(n+1)−a(n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* below offset – never used */

    /* 1‑based values stored at cache[n‑1] */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;               /* number of terms cached   */

    /* Grow the cache to contain at least n terms if needed */
    if (n > cache_size) {
        /* Ensure at least the first two terms for the recurrence */
        idx_t new_size = n;
        if (new_size < 2) new_size = 2;

        /* Allocate or enlarge the cache – works in C and C++ */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Initialise base values if the cache was empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)6727;
            cache[1] = (seq_t)163472827;
            cache_size = 2;
        }

        /* Compute any newly required terms using the linear recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)(((seq_t)24302) * cache[i - 1]) - cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
