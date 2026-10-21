/*
 * A239791  Number of compositions of n with no consecutive 2's.
 * https://oeis.org/A239791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed */
        cache = new_cache;
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) cache[0] = 1;
            else if (i == 1) cache[1] = 1;
            else if (i == 2) cache[2] = 2;
            else if (i == 3) cache[3] = 4;
            else if (i == 4) cache[4] = 7;
            else {
                seq_t v = 2 * cache[i-1];
                v -= cache[i-2];
                v += 2 * cache[i-3];
                v -= cache[i-4];
                v += cache[i-5];
                cache[i] = v;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
