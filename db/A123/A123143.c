/*
 * A123143  a(0)=0, a(1)=1, a(2)=2; a(3n) = a(n), a(3n+1) = a(n) + a(n+1),
 *          a(3n+2) = a(n+1) + a(n+2).
 * https://oeis.org/A123143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3n) = a(n), a(3n+1) = a(n)+a(n+1), a(3n+2) = a(n+1)+a(n+2),
 *            with a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely in tests */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)2;
            } else {
                idx_t k = i / 3;
                idx_t r = i % 3;
                if (r == 0) {
                    cache[i] = cache[k];
                } else if (r == 1) {
                    cache[i] = cache[k] + cache[k + 1];
                } else { /* r == 2 */
                    cache[i] = cache[k + 1] + cache[k + 2];
                }
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
