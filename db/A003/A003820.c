/*
 * A003820  a(1)=a(2)=1, a(n+1) = (a(n)^5 +1)/a(n-1).
 * https://oeis.org/A003820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1, a(n+1) = (a(n)^5 + 1) / a(n-1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }
    
    while (n > cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }
    
    for (idx_t i = 3; i <= n; i++) {
        if (cache[i - 1] == 0) {
            seq_t an1 = cache[i - 2];
            seq_t an2 = cache[i - 3];
            seq_t an1_5 = an1 * an1 * an1 * an1 * an1;
            cache[i - 1] = (an1_5 + 1) / an2;
        }
    }
    
    return cache[n - 1];
}
