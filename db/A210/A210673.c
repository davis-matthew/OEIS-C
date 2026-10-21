/*
 * A210673  a(n) = a(n-1)+a(n-2)+n-4, a(0)=0, a(1)=1.
 * https://oeis.org/A210673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=-1, a(3)=-1 a(n) = a(n-1) + a(n-2) + n - 4
 *            for n >= 4
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 48, i64 n <= 94, i128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache - static for all calls */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Compute the n-th term of OEIS A210673. */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* Allocate (or grow) the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failure – leave the function unusable;
               the build is expected to have sufficient memory. */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Fill in new terms, starting from the current cache size */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)-1;
            } else if (i == 3) {
                cache[i] = (seq_t)-1;
            } else {
                cache[i] = cache[i - 1] + cache[i - 2]
                           + (seq_t)i - (seq_t)4;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
