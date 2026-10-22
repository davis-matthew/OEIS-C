/*
 * A001006  Motzkin numbers: number of ways of drawing any number of
 *          nonintersecting chords joining n (labeled) points on a circle.
 * https://oeis.org/A001006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, changed
 * Formula:   a(n) = a(n-1) + sum_{k=0}^{n-2} a(k)*a(n-2-k) [Bernhart],
 *            a(0)=1, a(1)=1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    // Allocate cache on first call
    if (cache == NULL) {
        cache_size = (n < 100) ? 100 : (n + 1) * 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        memset(cache, 0, cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }
    
    // Grow cache if needed
    size_t old_size = cache_size;
    while (n >= cache_size) {
        cache_size *= 2;
    }
    if (cache_size > old_size) {
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        memset(cache + old_size, 0, (cache_size - old_size) * sizeof(seq_t));
    }
    
    // Compute values up to n
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] != 0) continue;
        
        seq_t sum = 0;
        for (idx_t k = 0; k < i - 1; k++) {
            sum = sum + cache[k] * cache[i - 2 - k];
        }
        cache[i] = cache[i - 1] + sum;
    }
    
    return cache[n];
}
