/*
 * A007289  Expansion of e.g.f. (sin(2*x) + cos(x)) / cos(3*x).
 * https://oeis.org/A007289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Sum_{k=0..floor((n-1)/2)} (-1)^k *
 *            binomial(n,2*k+1) * a(n-2*k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    // Allocate cache on first call
    if (cache == NULL) {
        cache_len = (n < 1000) ? 1000 : n + 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            cache[i] = 0;
        }
        computed_up_to = 0;
    }
    
    // Grow cache if needed
    while (n >= cache_len) {
        seq_t *new_cache = (seq_t *)malloc(cache_len * 2 * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = cache_len; i < cache_len * 2; i++) {
            new_cache[i] = 0;
        }
        free(cache);
        cache = new_cache;
        cache_len *= 2;
    }
    
    // Compute values from computed_up_to to n
    for (idx_t i = computed_up_to; i <= n; i++) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            idx_t limit = (i - 1) / 2;
            
            for (idx_t k = 0; k <= limit; k++) {
                seq_t binom_val = binom(i, 2 * k + 1);
                seq_t term = binom_val * cache[i - 2 * k - 1];
                
                if (k % 2 == 0) {
                    sum = sum + term;
                } else {
                    sum = sum - term;
                }
            }
            
            cache[i] = 2 * sum;
        }
    }
    
    if (n >= computed_up_to) {
        computed_up_to = n + 1;
    }
    
    return cache[n];
}
