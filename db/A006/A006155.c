/*
 * A006155  Expansion of e.g.f.: 1/(2-x-exp(x)).
 * https://oeis.org/A006155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *            a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (cache_size == 0) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = 1;
        
        for (idx_t i = 1; i <= n; i++) {
            seq_t term1 = (seq_t)i * cache[i - 1];
            seq_t sum = 0;
            for (idx_t k = 0; k < i; k++) {
                sum = sum + binom(i, k) * cache[k];
            }
            cache[i] = term1 + sum;
        }
        
        cache_size = n + 1;
        return cache[n];
    }
    
    if (n < cache_size) {
        return cache[n];
    }
    
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    
    for (idx_t i = cache_size; i <= n; i++) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t sum = 0;
        for (idx_t k = 0; k < i; k++) {
            sum = sum + binom(i, k) * cache[k];
        }
        cache[i] = term1 + sum;
    }
    
    cache_size = n + 1;
    return cache[n];
}
