/*
 * A007448  Knuth's sequence (or Knuth numbers): a(n+1) = 1 + min(
 *          2*a(floor(n/2)), 3*a(floor(n/3)) ).
 * https://oeis.org/A007448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n+1) = 1 + min(2*a(floor(n/2)), 3*a(floor(n/3))), a(0) = 1
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    idx_t *keys;
    seq_t *values;
    idx_t len;
    idx_t cap;
} Cache;

static Cache cache = {NULL, NULL, 0, 0};

static int find_in_cache(idx_t n) {
    for (idx_t i = 0; i < cache.len; i++) {
        if (cache.keys[i] == n) return i;
    }
    return -1;
}

static void add_to_cache(idx_t n, seq_t val) {
    if (cache.len == cache.cap) {
        cache.cap = cache.cap == 0 ? 16 : cache.cap * 2;
        cache.keys = (idx_t *)realloc(cache.keys, cache.cap * sizeof(idx_t));
        cache.values = (seq_t *)realloc(cache.values, cache.cap * sizeof(seq_t));
    }
    cache.keys[cache.len] = n;
    cache.values[cache.len] = val;
    cache.len++;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    int idx = find_in_cache(n);
    if (idx >= 0) return cache.values[idx];
    
    idx_t m = n - 1;
    seq_t val = 1 + MIN(2 * a(m / 2), 3 * a(m / 3));
    add_to_cache(n, val);
    return val;
}
