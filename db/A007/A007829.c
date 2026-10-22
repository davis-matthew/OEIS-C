/*
 * A007829  From random walks on complete directed triangle.
 * https://oeis.org/A007829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) - 4*a(n-4) + 5*a(n-5) +
 *            5*a(n-6) - 2*a(n-7) - 2*a(n-8) for n > 7 Initial: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=6, a(6)=8, a(7)=28
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 7;

seq_t a(idx_t n) {
    if (n <= 4) return 0;
    if (n == 5) return 6;
    if (n == 6) return 8;
    if (n == 7) return 28;
    
    static int initialized = 0;
    if (!initialized) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 6;
        cache[6] = 8;
        cache[7] = 28;
        initialized = 1;
    }
    
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = 2*cache[i-1] + 3*cache[i-2] - 6*cache[i-3] - 4*cache[i-4] 
                 + 5*cache[i-5] + 5*cache[i-6] - 2*cache[i-7] - 2*cache[i-8];
    }
    if (n > computed_up_to) computed_up_to = n;
    
    return cache[n];
}
