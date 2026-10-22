/*
 * A006844  a(1)=4, a(2)=5; thereafter a(n) is smallest number that is greater
 *          than a(n-1) and having a unique representation as a(j) + a(k) for
 *          j<k.
 * https://oeis.org/A006844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1)=4, a(2)=5; for n>2, a(n) is the smallest number > a(n-1)
 *            with unique representation as a(j) + a(k) for j < k; for n>9,
 *            a(n+32) = a(n) + 192
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 4;
    if (n == 2) return 5;
    
    if (cache == NULL) {
        cache_len = 100;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[1] = 4;
        cache[2] = 5;
        cache_computed = 2;
    }
    
    if (n > 41) {
        idx_t r = (n - 10) % 32;
        idx_t base_idx = r + 10;
        seq_t base_val = a(base_idx);
        idx_t k = (n - base_idx) / 32;
        seq_t offset = (seq_t)k * (seq_t)192;
        return base_val + offset;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }
    
    while (cache_computed < n) {
        idx_t i = cache_computed + 1;
        seq_t candidate = cache[i - 1] + 1;
        
        while (1) {
            int count = 0;
            for (idx_t j = 1; j < i; j++) {
                if (cache[j] >= candidate) break;
                seq_t needed = candidate - cache[j];
                for (idx_t k = j + 1; k < i; k++) {
                    if (cache[k] == needed) {
                        count++;
                        break;
                    }
                }
                if (count > 1) break;
            }
            if (count == 1) {
                cache[i] = candidate;
                break;
            }
            candidate++;
        }
        cache_computed++;
    }
    
    return cache[n];
}
