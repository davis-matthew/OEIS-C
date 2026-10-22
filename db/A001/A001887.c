/*
 * A001887  Number of permutations p of {1,2,...,n} such that p(i) - i < 0 or
 *          p(i) - i > 2 for all i.
 * https://oeis.org/A001887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (n-1)*a(n-1) + (n+2)*a(n-2) - (3*n-13)*a(n-3) - (2*n-8)*a(n-4) + (3*n-15)*a(n-5) + (n-4)*a(n-6) - (n-7)*a(n-7) - a(n-8), n>8 */
/* Initial values: a(0)=1, a(1..3)=0, a(4)=1, a(5)=5, a(6)=33, a(7)=236, a(8)=1918 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = (n < 128) ? 128 : n + 128;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        
        cache[0] = 1;
        cache[4] = 1;
        cache[5] = 5;
        cache[6] = 33;
        cache[7] = 236;
        cache[8] = 1918;
        
        computed_up_to = 8;
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 128;
        seq_t *new_cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        
        for (idx_t i = 0; i < old_len; i++) {
            new_cache[i] = cache[i];
        }
        
        free(cache);
        cache = new_cache;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t term1 = (seq_t)(i - 1) * cache[i - 1];
        seq_t term2 = (seq_t)(i + 2) * cache[i - 2];
        seq_t term3 = (seq_t)(3 * i - 13) * cache[i - 3];
        seq_t term4 = (seq_t)(2 * i - 8) * cache[i - 4];
        seq_t term5 = (seq_t)(3 * i - 15) * cache[i - 5];
        seq_t term6 = (seq_t)(i - 4) * cache[i - 6];
        seq_t term7 = (seq_t)(i - 7) * cache[i - 7];
        seq_t term8 = cache[i - 8];
        
        cache[i] = term1 + term2 - term3 - term4 + term5 + term6 - term7 - term8;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
