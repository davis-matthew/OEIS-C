/*
 * A074058  Reflected tetranacci numbers A073817.
 * https://oeis.org/A074058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1)-a(n-2)-a(n-3)+a(n-4), a(0)=4, a(1)=a(2)=a(3)=-1
 * Range:     i8 n <= 16, i16 n <= 41, i32 n <= 84, i64 n <= 171,
 *            i128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoization array */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Increase cache size if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – abort */
            abort();
        }
        cache = tmp;
        /* Initialize new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)        cache[i] = (seq_t)4;
            else if (i == 1)   cache[i] = (seq_t)-1;
            else if (i == 2)   cache[i] = (seq_t)-1;
            else if (i == 3)   cache[i] = (seq_t)-1;
            else {
                /* Compute using the recurrence */
                cache[i] = -cache[i-1] - cache[i-2] - cache[i-3] + cache[i-4];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
