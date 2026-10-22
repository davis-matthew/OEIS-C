/*
 * A003663  a(n) is smallest number != a(j) + a(k), j < k and a(1) = 1, a(2) =
 *          6.
 * https://oeis.org/A003663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 9, a(1)=1, a(2)=6,
 *            a(3)=8, a(4)=10, a(5)=12, a(6)=15, a(7)=17, a(8)=19, a(9)=24
 * Range:     u8 n <= 86, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)malloc(9 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 8;
        cache[3] = 10;
        cache[4] = 12;
        cache[5] = 15;
        cache[6] = 17;
        cache[7] = 19;
        cache[8] = 24;
    }
    
    while (cache_len < n) {
        idx_t old_len = cache_len;
        idx_t new_len = cache_len * 2;
        
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i < new_len; i++) {
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
        
        cache_len = new_len;
    }
    
    return cache[n - 1];
}
