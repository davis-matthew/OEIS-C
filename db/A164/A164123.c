/*
 * A164123  Partial sums of A162436.
 * https://oeis.org/A164123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 4 (n>2), a(1)=1, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; 0 not used */

    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    if (n > cache_size) {
        idx_t new_size = n;
        if (cache == NULL) {
            cache = (seq_t*)malloc((new_size + 1) * sizeof(seq_t));
            if (!cache) abort();
            cache[0] = 0;               /* unused */
            cache[1] = (seq_t)1;
            if (new_size >= 2) cache[2] = (seq_t)4;
            for (idx_t i = 3; i <= new_size; ++i)
                cache[i] = cache[i - 2] * (seq_t)3 + (seq_t)4;
            cache_size = new_size;
        } else {
            seq_t *tmp = (seq_t*)realloc(cache,
                                         (new_size + 1) * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            for (idx_t i = cache_size + 1; i <= new_size; ++i) {
                if (i == 1) cache[1] = (seq_t)1;
                else if (i == 2) cache[2] = (seq_t)4;
                else cache[i] = cache[i - 2] * (seq_t)3 + (seq_t)4;
            }
            cache_size = new_size;
        }
    }
    return cache[n];
}
