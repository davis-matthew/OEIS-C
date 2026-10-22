/*
 * A001612  a(n) = a(n-1) + a(n-2) - 1 for n > 1, a(0)=3, a(1)=2.
 * https://oeis.org/A001612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = a(n-1) + a(n-2) - 1, a(0) = 3, a(1) = 2
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[10001];
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    // Initialize on first call
    if (cache_len == 0) {
        cache[0] = 3;
        cache[1] = 2;
        cache_len = 2;
    }
    
    // If n is within cache range and already computed
    if (n < cache_len) {
        return cache[n];
    }
    
    // Need to compute more values
    if (n <= 10000) {
        // Compute and cache up to n
        for (idx_t i = cache_len; i <= n; i++) {
            cache[i] = cache[i-1] + cache[i-2] - 1;
        }
        cache_len = n + 1;
        return cache[n];
    } else {
        // Compute and cache up to 10000, then compute beyond
        if (cache_len <= 10000) {
            for (idx_t i = cache_len; i <= 10000; i++) {
                cache[i] = cache[i-1] + cache[i-2] - 1;
            }
            cache_len = 10001;
        }
        
        // Now compute from 10001 to n
        seq_t prev2 = cache[9999];
        seq_t prev1 = cache[10000];
        
        for (idx_t i = 10001; i <= n; i++) {
            seq_t curr = prev1 + prev2 - 1;
            prev2 = prev1;
            prev1 = curr;
        }
        
        return prev1;
    }
}
