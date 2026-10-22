/*
 * A006279  Denominators of convergents to Cahen's constant: a(n+2) =
 *          a(n)^2*a(n+1) + a(n).
 * https://oeis.org/A006279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n+2) = a(n)^2*a(n+1) + a(n), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t max_computed = 0;
    
    if (cache == NULL) {
        idx_t init_len = (n < 2) ? 2 : (n + 1);
        cache = grow_zero(NULL, 0, init_len);
        cache_len = init_len;
        cache[0] = 1;
        cache[1] = 1;
        max_computed = 1;
    } else if (n >= cache_len) {
        cache = grow_zero(cache, cache_len, n + 1);
        cache_len = n + 1;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        seq_t prev2 = cache[i - 2];
        seq_t prev1 = cache[i - 1];
        cache[i] = prev2 * prev2 * prev1 + prev2;
    }
    
    if (n > max_computed) {
        max_computed = n;
    }
    
    return cache[n];
}
