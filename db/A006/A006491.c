/*
 * A006491  Generalized Lucas numbers.
 * https://oeis.org/A006491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6), a(1)=1, a(2)=0,
 *            a(3)=4, a(4)=5, a(5)=15, a(6)=28
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 81, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        idx_t new_len = (n < 1000) ? 1000 : (n + 1) * 2;
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        cache_len = new_len;
        
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)15;
        cache[6] = (seq_t)28;
        computed_up_to = 6;
    } else if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t v = (seq_t)3 * cache[i-1] - (seq_t)5 * cache[i-3] + (seq_t)3 * cache[i-5] + cache[i-6];
        cache[i] = v;
    }
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return cache[n];
}
