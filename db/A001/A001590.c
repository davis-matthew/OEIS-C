/*
 * A001590  Tribonacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) with a(0)=0,
 *          a(1)=1, a(2)=0.
 * https://oeis.org/A001590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 0, a(1) = 1, a(2) = 0
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 2;
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 0;
    
    if (cache == NULL) {
        cache_len = 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
    }
    
    while (cache_len <= n) {
        cache = (seq_t *)realloc(cache, (cache_len * 2) * sizeof(seq_t));
        cache_len *= 2;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return cache[n];
}
