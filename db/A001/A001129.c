/*
 * A001129  Iccanobif numbers: reverse digits of two previous terms and add.
 * https://oeis.org/A001129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = reverse_digits(a(n-2)) + reverse_digits(a(n-1)), a(0) =
 *            0, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 38, u64 n <= 65, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t reverse_digits(seq_t x) {
    if (x == 0) return 0;
    
    seq_t result = 0;
    while (x > 0) {
        result = result * 10 + (x % 10);
        x /= 10;
    }
    return result;
}

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    
    if (cache == NULL) {
        cache_cap = (n > 100) ? n + 100 : 200;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;
    }
    
    if (n >= cache_cap) {
        idx_t new_cap = n + 100;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = reverse_digits(cache[i-2]) + reverse_digits(cache[i-1]);
    }
    
    if (n >= cache_len) {
        cache_len = n + 1;
    }
    
    return cache[n];
}
