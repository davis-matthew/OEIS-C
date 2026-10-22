/*
 * A003735  Number of perfect matchings (or domino tilings) in W_5 X P_2n.
 * https://oeis.org/A003735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44a(n-1) - 102a(n-2) + 44a(n-3) - a(n-4), a(1)=29,
 *            a(2)=1189, a(3)=49401, a(4)=2053641
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (n <= 0) return 0;
    
    if (cache == NULL) {
        cache_size = 1024;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[1] = 29;
        cache[2] = 1189;
        cache[3] = 49401;
        cache[4] = 2053641;
        for (idx_t i = 5; i < cache_size; ++i) {
            cache[i] = (seq_t)44 * cache[i-1] - (seq_t)102 * cache[i-2] + (seq_t)44 * cache[i-3] - cache[i-4];
        }
    } else if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1024;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        for (idx_t i = old_size; i < cache_size; ++i) {
            cache[i] = (seq_t)44 * cache[i-1] - (seq_t)102 * cache[i-2] + (seq_t)44 * cache[i-3] - cache[i-4];
        }
    }
    
    return cache[n];
}
