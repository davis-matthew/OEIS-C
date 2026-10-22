/*
 * A004484  Sprague-Grundy values for game of Wyt Queens.
 * https://oeis.org/A004484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) for n > 14, with base cases:
 *            a(0)=3, a(1)=4, a(2)=5, a(3)=6, a(4)=2, a(5)=0, a(6)=1, a(7)=9,
 *            a(8)=10, a(9)=12, a(10)=8, a(11)=7, a(12)=15, a(13)=11, a(14)=16
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[] = {3, 4, 5, 6, 2, 0, 1, 9, 10, 12, 8, 7, 15, 11, 16};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_computed_up_to = 14;

static seq_t a_base(idx_t n) {
    if (n < 15) {
        return base_values[n];
    }
    return cache[n - 15];
}

seq_t a(idx_t n) {
    if (n < 15) {
        return base_values[n];
    }
    
    if (n <= cache_computed_up_to) {
        return cache[n - 15];
    }
    
    idx_t idx = n - 15;
    if (idx >= cache_len) {
        idx_t new_len = (idx + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = cache_computed_up_to + 1; i <= n; i++) {
        seq_t val = a_base(i - 1) + a_base(i - 6) - a_base(i - 7);
        cache[i - 15] = val;
    }
    
    cache_computed_up_to = n;
    return cache[n - 15];
}
