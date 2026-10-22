/*
 * A000360  Distribution of nonempty triangles inside a fractal rep-4-tile.
 * https://oeis.org/A000360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, eigen, look, changed
 * Formula:   a(0) = 1, a(2n) = a(n) + a(n-1), a(2n+1) = a(n) + 1 - ((n-1) mod
 *            3)
 * Range:     u8 n <= 9555, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_len = 0;

static void ensure_cached(idx_t n) {
    if (n < cache_len) return;
    
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    bool *new_computed = (bool *)realloc(computed, new_len * sizeof(bool));
    
    if (new_cache == NULL || new_computed == NULL) {
        return;
    }
    
    cache = new_cache;
    computed = new_computed;
    
    for (idx_t i = cache_len; i < new_len; i++) {
        computed[i] = false;
    }
    
    cache_len = new_len;
}

seq_t a(idx_t n) {
    ensure_cached(n);
    
    if (computed[n]) {
        return cache[n];
    }
    
    seq_t result;
    if (n == 0) {
        result = 1;
    } else if (n % 2 == 0) {
        idx_t k = n / 2;
        result = a(k) + a(k - 1);
    } else {
        idx_t k = n / 2;
        seq_t km1_mod3;
        if (k == 0) {
            km1_mod3 = 2;
        } else {
            km1_mod3 = (seq_t)((k - 1) % 3);
        }
        result = a(k) + 1 - km1_mod3;
    }
    
    cache[n] = result;
    computed[n] = true;
    return result;
}
