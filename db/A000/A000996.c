/*
 * A000996  Shifts 3 places left under binomial transform.
 * https://oeis.org/A000996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Σ_{k=0}^{n-3} C(n-3, k) * a(k); a(0)=1, a(1)=a(2)=0
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 36, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        cache = new_cache;
        
        for (idx_t i = cache_size; i < new_size; i++) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1 || i == 2) {
                cache[i] = 0;
            } else {
                seq_t sum = 0;
                idx_t m = i - 3;
                seq_t b = 1;
                
                for (idx_t k = 0; k <= m; k++) {
                    sum += b * cache[k];
                    if (k < m) {
                        b = (b * (seq_t)(m - k)) / (seq_t)(k + 1);
                    }
                }
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }
    
    return cache[n];
}
