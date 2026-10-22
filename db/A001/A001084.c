/*
 * A001084  a(n) = 20*a(n-1) - a(n-2) with a(0) = 0, a(1) = 3.
 * https://oeis.org/A001084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - a(n-2), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static int cache_init = 0;

static void ensure_cache(idx_t n) {
    if (!cache_init) {
        cache[0] = 0;
        cache[1] = 3;
        cache_init = 2;
    }
    
    if (n >= CACHE_SIZE) n = CACHE_SIZE - 1;
    
    while (cache_init <= n) {
        cache[cache_init] = 20 * cache[cache_init - 1] - cache[cache_init - 2];
        cache_init++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;
    
    if (n < CACHE_SIZE) {
        ensure_cache(n);
        return cache[n];
    }
    
    ensure_cache(CACHE_SIZE - 1);
    
    seq_t prev2 = cache[CACHE_SIZE - 2];
    seq_t prev1 = cache[CACHE_SIZE - 1];
    
    for (idx_t i = CACHE_SIZE; i <= n; i++) {
        seq_t curr = 20 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
