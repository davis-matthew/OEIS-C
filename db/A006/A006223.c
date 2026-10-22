/*
 * A006223  Number of binary rooted trees of height n requiring 3 registers.
 * https://oeis.org/A006223
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 78*a(n-2) + 220*a(n-3) - 330*a(n-4) +
 *            252*a(n-5) - 84*a(n-6) + 8*a(n-7) a(1) = ... = a(6) = 0, a(7) =
 *            1
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n < 7) return 0;
    if (n == 7) return 1;
    
    if (n <= max_computed) {
        return cache[n];
    }
    
    if (cache == NULL) {
        cache_len = (n + 100 > 100) ? n + 100 : 100;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[7] = 1;
        max_computed = 7;
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 100;
        seq_t *new_cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        memcpy(new_cache, cache, old_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 14 * cache[i-1]
                 - 78 * cache[i-2]
                 + 220 * cache[i-3]
                 - 330 * cache[i-4]
                 + 252 * cache[i-5]
                 - 84 * cache[i-6]
                 + 8 * cache[i-7];
    }
    max_computed = n;
    
    return cache[n];
}
