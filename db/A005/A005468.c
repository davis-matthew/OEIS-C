/*
 * A005468  a(n) = 1 + a(floor(n/2))*a(ceiling(n/2)).
 * https://oeis.org/A005468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, changed
 * Formula:   a(n) = 1 + a(floor(n/2)) * a(ceiling(n/2))
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 55, u64 n <= 111,
 *            u128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

#define CACHE_SIZE 100000

typedef struct {
    idx_t n;
    seq_t value;
} CacheEntry;

static CacheEntry cache[CACHE_SIZE];
static int cache_init = 0;

static seq_t a_impl(idx_t n);

static seq_t lookup(idx_t n) {
    idx_t hash = n % CACHE_SIZE;
    for (idx_t i = 0; i < CACHE_SIZE; i++) {
        idx_t pos = (hash + i) % CACHE_SIZE;
        if (cache[pos].n == n) {
            return cache[pos].value;
        }
        if (cache[pos].n == 0) {
            return 0;
        }
    }
    return 0;
}

static void store(idx_t n, seq_t v) {
    idx_t hash = n % CACHE_SIZE;
    for (idx_t i = 0; i < CACHE_SIZE; i++) {
        idx_t pos = (hash + i) % CACHE_SIZE;
        if (cache[pos].n == n || cache[pos].n == 0) {
            cache[pos].n = n;
            cache[pos].value = v;
            return;
        }
    }
}

static seq_t a_impl(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    
    seq_t cached = lookup(n);
    if (cached != 0) {
        return cached;
    }
    
    idx_t half = n / 2;
    idx_t half_ceil = (n + 1) / 2;
    
    seq_t result = (seq_t)1 + a_impl(half) * a_impl(half_ceil);
    store(n, result);
    return result;
}

seq_t a(idx_t n) {
    if (!cache_init) {
        memset(cache, 0, sizeof(cache));
        cache_init = 1;
    }
    return a_impl(n);
}
