/*
 * A206687  Number of n X 2 0..3 arrays with no element equal to another
 *          within two positions in the same row or column, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206687
   a(n) = 2*a(n-1) + 4*a(n-2)   for n > 4
   a(1) = 1, a(2) = 4, a(3) = 11, a(4) = 36
   memoised, works for any n >= 1
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index currently stored */

    if (n == 0)
        return 0;        /* offset is 1 – this case is unused */

    if (n > cache_size) {
        size_t new_size = (size_t)(n + 1);      /* index 0 is unused */
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
            cache[0] = 0;                       /* unused */
        } else {
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)4;
            else if (i == 3)
                cache[i] = (seq_t)11;
            else if (i == 4)
                cache[i] = (seq_t)36;
            else
                cache[i] = cache[i - 1] * ((seq_t)2)
                           + cache[i - 2] * ((seq_t)4);
        }
        cache_size = n;
    }

    return cache[n];
}
