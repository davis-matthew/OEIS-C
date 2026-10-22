/*
 * A003736  Number of 2-factors in W_5 X P_n.
 * https://oeis.org/A003736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) + 149*a(n-2) - 285*a(n-3) - 1354*a(n-4) +
 *            1098*a(n-5) - 24*a(n-6) Base cases: a(1) = 4, a(2) = 156, a(3) =
 *            3832, a(4) = 101476, a(5) = 2653176, a(6) = 69537644
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t last_computed = 0;

seq_t a(idx_t n) {
    if (n == 1) return 4;
    if (n == 2) return 156;
    if (n == 3) return 3832;
    if (n == 4) return 101476;
    if (n == 5) return 2653176;
    if (n == 6) return 69537644;
    
    if (cache == NULL) {
        cache_len = (n > 100) ? n + 100 : 200;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        cache[1] = 4;
        cache[2] = 156;
        cache[3] = 3832;
        cache[4] = 101476;
        cache[5] = 2653176;
        cache[6] = 69537644;
        last_computed = 6;
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 100;
        seq_t *new_cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= old_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
    }
    
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; i++) {
            cache[i] = (seq_t)21 * cache[i-1] + (seq_t)149 * cache[i-2] 
                     - (seq_t)285 * cache[i-3] - (seq_t)1354 * cache[i-4] 
                     + (seq_t)1098 * cache[i-5] - (seq_t)24 * cache[i-6];
        }
        last_computed = n;
    }
    
    return cache[n];
}
