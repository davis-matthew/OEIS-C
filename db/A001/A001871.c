/*
 * A001871  Expansion of 1/(1 - 3*x + x^2)^2.
 * https://oeis.org/A001871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4), a(0) = 1, a(1)
 *            = 6, a(2) = 25, a(3) = 90
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static idx_t cached_up_to = 0;
static int cache_init = 0;

seq_t a(idx_t n) {
    if (!cache_init) {
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 25;
        cache[3] = 90;
        cached_up_to = 3;
        cache_init = 1;
    }
    
    // For n within cache bounds
    if (n < CACHE_SIZE) {
        if (n > cached_up_to) {
            for (idx_t i = cached_up_to + 1; i <= n; i++) {
                cache[i] = 6 * cache[i-1] - 11 * cache[i-2] + 6 * cache[i-3] - cache[i-4];
            }
            cached_up_to = n;
        }
        return cache[n];
    }
    
    // For n >= CACHE_SIZE, ensure cache is fully computed
    if (cached_up_to < CACHE_SIZE - 1) {
        for (idx_t i = cached_up_to + 1; i < CACHE_SIZE; i++) {
            cache[i] = 6 * cache[i-1] - 11 * cache[i-2] + 6 * cache[i-3] - cache[i-4];
        }
        cached_up_to = CACHE_SIZE - 1;
    }
    
    // Compute from cache boundary to n using rolling window
    seq_t v0 = cache[CACHE_SIZE - 4];
    seq_t v1 = cache[CACHE_SIZE - 3];
    seq_t v2 = cache[CACHE_SIZE - 2];
    seq_t v3 = cache[CACHE_SIZE - 1];
    
    for (idx_t i = CACHE_SIZE; i <= n; i++) {
        seq_t v_new = 6 * v3 - 11 * v2 + 6 * v1 - v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v_new;
    }
    
    return v3;
}
