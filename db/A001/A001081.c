/*
 * A001081  a(n) = 16*a(n-1) - a(n-2).
 * https://oeis.org/A001081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE 10001
static seq_t cache[MAX_CACHE];
static int cache_ready = 0;

seq_t a(idx_t n) {
    if (!cache_ready) {
        cache[0] = 1;
        cache[1] = 8;
        for (int i = 2; i < MAX_CACHE; i++) {
            cache[i] = 16 * cache[i-1] - cache[i-2];
        }
        cache_ready = 1;
    }
    
    if (n < MAX_CACHE) {
        return cache[n];
    }
    
    seq_t prev2 = cache[MAX_CACHE - 2];
    seq_t prev1 = cache[MAX_CACHE - 1];
    
    for (idx_t i = MAX_CACHE; i <= n; i++) {
        seq_t curr = 16 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
