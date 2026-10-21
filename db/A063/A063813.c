/*
 * A063813  Growth series for fundamental group of orientable closed surface
 *          of genus 3.
 * https://oeis.org/A063813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+10*a(n-2)+10*a(n-3)+10*a(n-4)+10*a(n-5)-a(n-6)
 *            for n>6 with initial terms a(0)=1, a(1)=12, a(2)=132, a(3)=1452,
 *            a(4)=15972, a(5)=175692, a(6)=1932600
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;    /* array of computed values      */
    static idx_t cached_size = 0;  /* number of elements stored     */
    static size_t capacity = 0;    /* allocated capacity in entries  */

    /* Initialise cache with the first seven terms */
    if (cache == NULL) {
        capacity = 7;                     /* minimal capacity   */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return (seq_t)0;       /* allocation failure */

        cache[0] = 1;
        cache[1] = 12;
        cache[2] = 132;
        cache[3] = 1452;
        cache[4] = 15972;
        cache[5] = 175692;
        cache[6] = 1932600;

        cached_size = 7;
    }

    /* Extend cache if a larger index is requested */
    if ((size_t)n >= cached_size) {
        size_t needed = (size_t)n + 1;

        /* Expand storage as needed */
        if (needed > capacity) {
            while (capacity < needed) capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
            if (!tmp) return (seq_t)0;     /* allocation failure */
            cache = tmp;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t term =
                10 * cache[i-1] +
                10 * cache[i-2] +
                10 * cache[i-3] +
                10 * cache[i-4] +
                10 * cache[i-5] -
                cache[i-6];
            cache[i] = term;
        }

        cached_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
