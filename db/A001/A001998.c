/*
 * A001998  Bending a piece of wire of length n+1; walks of length n+1 on a
 *          tetrahedron; also non-branched catafusenes with n+2 condensed
 *          hexagons.
 * https://oeis.org/A001998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*a(n-1) - 12*a(n-3) + 9*a(n-4), a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=10
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = mem_new(seq_t, 4);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 10;
        cache_len = 4;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    cache = mem_resize(seq_t, cache, (n + 1));
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = 4 * cache[i-1] - 12 * cache[i-3] + 9 * cache[i-4];
    }
    
    cache_len = n + 1;
    return cache[n];
}
