/*
 * A236011  Number of (n+1) X (1+1) 0..2 arrays colored with the sum of the
 *          maximum and minimum values of each 2 X 2 subblock.
 * https://oeis.org/A236011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-2) + 14*a(n-3) + 30*a(n-4) + 16*a(n-5) + 8*a(n-6)
 *            for n > 7, with a(1)=81, a(2)=306, a(3)=1276, a(4)=4672,
 *            a(5)=18784, a(6)=70688, a(7)=278352
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* Initial values for n = 1..7 */
    static const seq_t init[7] = {
        (seq_t)81, (seq_t)306, (seq_t)1276,
        (seq_t)4672, (seq_t)18784, (seq_t)70688,
        (seq_t)278352
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* next unused index (number of values stored) */

    if (n == 0) {
        /* Offset is 1; this case is not used for valid input. */
        return (seq_t)0;
    }

    /* Convert the index to a size_t so it can be used as an array index. */
    size_t target = (size_t)n;          /* number of values needed */
    size_t zero_index = target - 1;     /* index of a(n) in the zero‑based array */

    /* Grow the cache if necessary */
    if (target > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : target;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* For the purpose of this problem we assume realloc succeeds. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any values that have not yet been cached */
    for (size_t i = cache_len; i < target; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            cache[i] = (seq_t)( 9   * cache[i - 2]
                              + 14  * cache[i - 3]
                              + 30  * cache[i - 4]
                              + 16  * cache[i - 5]
                              + 8   * cache[i - 6] );
        }
    }
    cache_len = target;

    return cache[zero_index];
}
