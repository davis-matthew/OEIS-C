/*
 * A006894  Number of planted 3-trees of height < n.
 * https://oeis.org/A006894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = a(n-1) * (a(n-1) + 1) / 2 + 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        cache[0] = 1;
        cache_len = 1;
    }
    
    if (n <= cache_len) {
        return cache[n - 1];
    }
    
    seq_t *new_cache = mem_resize(seq_t, cache, n);
    cache = new_cache;
    
    for (idx_t i = cache_len + 1; i <= n; i++) {
        seq_t prev = cache[i - 2];
        cache[i - 1] = prev * (prev + 1) / 2 + 1;
    }
    
    cache_len = n;
    return cache[n - 1];
}
