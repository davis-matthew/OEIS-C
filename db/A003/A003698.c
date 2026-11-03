/*
 * A003698  Number of 2-factors in C_4 X P_n.
 * https://oeis.org/A003698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 4*a(n-3) for n>3; a(1)=1, a(2)=9,
 *            a(3)=53
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 883 (2026-11-03)
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
        cache = mem_new(seq_t, 4);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 9;
        cache[3] = 53;
        cache_len = 4;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    seq_t *new_cache = mem_resize(seq_t, cache, (n + 1));
    cache = new_cache;
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = 6 * cache[i-1] + 3 * cache[i-2] - 4 * cache[i-3];
    }
    
    cache_len = n + 1;
    return cache[n];
}
