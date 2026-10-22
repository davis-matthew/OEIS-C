/*
 * A005229  a(1) = a(2) = 1; for n > 2, a(n) = a(a(n-2)) + a(n - a(n-2)).
 * https://oeis.org/A005229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(a(n-2)) + a(n - a(n-2)), a(1) = a(2) = 1
 * Range:     u8 n <= 370, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = 0;
static idx_t cache_len = 0; /* cache[1..cache_len-1] are filled */

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (new_len < 2 * cache_len) new_len = 2 * cache_len;
        uint64_t *p = (uint64_t *)realloc(cache, (size_t)new_len * sizeof(uint64_t));
        if (!p) return (seq_t)0;
        cache = p;
        idx_t start = cache_len;
        if (start < 3) {
            cache[0] = 0;
            cache[1] = 1;
            cache[2] = 1;
            start = 3;
        }
        for (idx_t k = start; k < new_len; k++) {
            uint64_t t = cache[k - 2];
            cache[k] = cache[t] + cache[k - t];
        }
        cache_len = new_len;
    }
    return (seq_t)cache[n];
}
