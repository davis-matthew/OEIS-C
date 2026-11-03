/*
 * A005691  Number of Twopins positions.
 * https://oeis.org/A005691
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) + 2*a(n-6) - 2*a(n-7) -
 *            a(n-12) for n >= 18
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 63, u64 n <= 121,
 *            u128 n <= 238, big any n
 * Validated: standard, pass, 60 terms through n = 992 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n < 6) {
        return (n == 0) ? 0 : 1;
    }
    
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;
    
    long long n_ll = (long long)n;
    
    if (cache == NULL) {
        cache_size = 100;
        cache = mem_new(seq_t, (size_t)cache_size);
        
        cache[0] = 0;
        for (idx_t i = 1; i < 6; i++) cache[i] = 1;
        cache[6] = 1;
        cache[7] = 2;
        cache[8] = 3;
        cache[9] = 5;
        cache[10] = 7;
        cache[11] = 10;
        cache[12] = 13;
        cache[13] = 18;
        cache[14] = 24;
        cache[15] = 35;
        cache[16] = 50;
        cache[17] = 75;
        computed_up_to = 17;
    }
    
    if (n_ll > computed_up_to) {
        while (n_ll >= cache_size) {
            cache_size *= 2;
            cache = mem_resize(seq_t, cache, (size_t)cache_size);
        }
        
        for (long long i = computed_up_to + 1; i <= n_ll; i++) {
            cache[i] = 2 * cache[i - 1]
                     - 2 * cache[i - 3]
                     + cache[i - 4]
                     + 2 * cache[i - 6]
                     - 2 * cache[i - 7]
                     - cache[i - 12];
        }
        computed_up_to = n_ll;
    }
    
    return cache[n_ll];
}
