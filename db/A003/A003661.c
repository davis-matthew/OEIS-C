/*
 * A003661  Number of bipartite Steinhaus graphs on n nodes.
 * https://oeis.org/A003661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k+1)=2*a(k+1)+1; a(2k)=a(k)+a(k+1) for k>=2; base cases:
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
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
    if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        if (new_len < 16) new_len = 16;
        
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n > 0 && cache[n] != 0) {
        return cache[n];
    }
    
    seq_t result;
    if (n == 1) {
        result = 1;
    } else if (n == 2) {
        result = 2;
    } else if (n == 3) {
        result = 4;
    } else if (n % 2 == 0) {
        result = a(n / 2) + a(n / 2 + 1);
    } else {
        result = 2 * a((n + 1) / 2) + 1;
    }
    
    if (n > 0) {
        cache[n] = result;
    }
    return result;
}
