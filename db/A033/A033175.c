/*
 * A033175  n 3's followed by 1.
 * https://oeis.org/A033175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + 21, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of elements cached so far */

    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                cache[i] = cache[i-1] * (seq_t)10 + (seq_t)21;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
