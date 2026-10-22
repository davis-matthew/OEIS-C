/*
 * A007554  Unique attractor for (RIGHT then MOBIUS) transform.
 * https://oeis.org/A007554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, eigen, look
 * Formula:   a(n+1) = Sum_{d|n} mu(n/d) * a(d)
 * Range:     i8 n <= 48, i16 n <= 274, i32 n <= 1592, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int mobius(uint64_t n) {
    if (n == 1) return 1;
    
    int mu = 1;
    uint64_t d = 2;
    
    while (d * d <= n) {
        if (n % d == 0) {
            if (n % (d * d) == 0) return 0;
            mu = -mu;
            n /= d;
        } else {
            d++;
        }
    }
    
    if (n > 1) mu = -mu;
    
    return mu;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        computed_up_to = 1;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len < n) new_len *= 2;
        
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t result = 0;
        uint64_t m = (uint64_t)i - 1;
        
        for (uint64_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                int mu = mobius((uint64_t)(m / d));
                result += (seq_t)mu * cache[(idx_t)d - 1];
                
                if (d != m / d) {
                    mu = mobius(d);
                    result += (seq_t)mu * cache[(idx_t)(m / d) - 1];
                }
            }
        }
        
        cache[(idx_t)i - 1] = result;
        computed_up_to = i;
    }
    
    return cache[(idx_t)n - 1];
}
