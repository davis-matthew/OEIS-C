/*
 * A001949  Solutions of a fifth-order probability difference equation.
 * https://oeis.org/A001949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) for n >= 5
 *            Base cases: a(0) = a(1) = a(2) = a(3) = a(4) = 0
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 70, u128 n <= 136,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"


seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t last_computed = 4;
    
    if (cache == NULL) {
        cache = a001028_z(6);
        cache_len = 6;
    }
    
    if (n >= cache_len) {
        cache = a001028_grow(cache, cache_len, n + 1);
        cache_len = n + 1;
    }
    
    for (idx_t i = last_computed + 1; i <= n; i++) {
        cache[i] = (seq_t)1 + cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
    }
    
    if (n > last_computed) {
        last_computed = n;
    }
    
    return cache[n];
}
