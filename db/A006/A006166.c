/*
 * A006166  a(0)=0, a(1)=a(2)=1; for n >= 1, a(3n+2) = 2a(n+1) + a(n), a(3n+1)
 *          = a(n+1) + 2a(n), a(3n) = 3a(n).
 * https://oeis.org/A006166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=a(2)=1; for n >= 1, a(3n)=3a(n),
 *            a(3n+1)=a(n+1)+2a(n), a(3n+2)=2a(n+1)+a(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_SIZE 10000

typedef struct {
    idx_t key;
    seq_t value;
    bool used;
} CacheEntry;

static CacheEntry cache[CACHE_SIZE];

static seq_t cached_compute(idx_t n);

static seq_t lookup_or_compute(idx_t n) {
    idx_t h = n % CACHE_SIZE;
    for (idx_t i = 0; i < CACHE_SIZE; i++) {
        idx_t pos = (h + i) % CACHE_SIZE;
        if (!cache[pos].used) {
            cache[pos].key = n;
            cache[pos].value = cached_compute(n);
            cache[pos].used = true;
            return cache[pos].value;
        }
        if (cache[pos].key == n) {
            return cache[pos].value;
        }
    }
    return cached_compute(n);
}

static seq_t cached_compute(idx_t n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;
    
    idx_t m = n / 3;
    idx_t r = n % 3;
    
    if (r == 0) {
        return 3 * lookup_or_compute(m);
    } else if (r == 1) {
        return lookup_or_compute(m + 1) + 2 * lookup_or_compute(m);
    } else {
        return 2 * lookup_or_compute(m + 1) + lookup_or_compute(m);
    }
}

seq_t a(idx_t n) {
    return lookup_or_compute(n);
}
