/*
 * A003753  Number of spanning trees in C_4 X P_n.
 * https://oeis.org/A003753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 90a(n-1) - 735a(n-2) + 1548a(n-3) - 735a(n-4) + 90a(n-5)
 *            - a(n-6), a(1) = 4, a(2) = 384, a(3) = 31500, a(4) = 2558976,
 *            a(5) = 207746836, a(6) = 16864848000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

static seq_t base_vals[] = {0, 4, 384, 31500, 2558976, 207746836, 16864848000};
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n < 7) return base_vals[n];
    
    if (cache == NULL) {
        cache_len = (n < 1000) ? 1000 : n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memcpy(cache, base_vals, 7 * sizeof(seq_t));
        memset(cache + 7, 0, (cache_len - 7) * sizeof(seq_t));
        max_computed = 6;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = (seq_t)90 * cache[i-1]
                 - (seq_t)735 * cache[i-2]
                 + (seq_t)1548 * cache[i-3]
                 - (seq_t)735 * cache[i-4]
                 + (seq_t)90 * cache[i-5]
                 - cache[i-6];
    }
    if (n > max_computed) max_computed = n;
    
    return cache[n];
}
