/*
 * A143454  Expansion of 1/(x^k*(1 - x - 3*x^(k+1))) for k=3.
 * https://oeis.org/A143454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=7, a(3)=10; a(n)=a(n-1)+3*a(n-4) for n>=4
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* compute how many elements are required */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_size) {
        /* allocate a larger array */
        seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – terminate */
            abort();
        }
        /* initialise new entries */
        for (size_t i = 0; i < needed; ++i) {
            if (i == 0) {
                new_cache[0] = (seq_t)1;
            } else if (i == 1) {
                new_cache[1] = (seq_t)4;
            } else if (i == 2) {
                new_cache[2] = (seq_t)7;
            } else if (i == 3) {
                new_cache[3] = (seq_t)10;
            } else {
                /* recurrence: a(i) = a(i-1) + 3*a(i-4) */
                new_cache[i] = new_cache[i-1] + (new_cache[i-4] * (seq_t)3);
            }
        }
        /* free old cache if any and adopt the new one */
        free(cache);
        cache = new_cache;
        cache_size = needed;
    }
    return cache[(size_t)n];
}
