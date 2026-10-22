/*
 * A003995  Sum of (any number of) distinct squares: of form r^2 + s^2 + t^2 +
 *          ... with 0 <= r < s < t < ...
 * https://oeis.org/A003995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Exponents in expansion of (1+x)*(1+x^4)*(1+x^9)*(1+x^16)*... =
 *            sum of distinct squares. For n > 98, a(n) = n + 30
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n > 98) {
        return (seq_t)n + 30;
    }
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        idx_t limit = 500;
        
        int *reachable = (int *)calloc(limit + 1, sizeof(int));
        reachable[0] = 1;
        
        for (idx_t k = 1; k * k <= limit; k++) {
            idx_t k_sq = k * k;
            for (idx_t s = limit; s >= k_sq; s--) {
                if (reachable[s - k_sq]) {
                    reachable[s] = 1;
                }
            }
        }
        
        idx_t count = 0;
        for (idx_t s = 0; s <= limit; s++) {
            if (reachable[s]) count++;
        }
        
        cache = (seq_t *)malloc(count * sizeof(seq_t));
        cache_len = count;
        idx_t idx = 0;
        for (idx_t s = 0; s <= limit; s++) {
            if (reachable[s]) {
                cache[idx++] = (seq_t)s;
            }
        }
        
        free(reachable);
    }
    
    if (n <= cache_len) {
        return cache[n - 1];
    }
    
    return (seq_t)n + 30;
}
