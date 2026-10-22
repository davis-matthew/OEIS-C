/*
 * A006158  a(n) = a(a(n-3)) + a(n-a(n-3)).
 * https://oeis.org/A006158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-3)) + a(n-a(n-3)), a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 493, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Terms satisfy a(n) <= n, so they are memoized as native uint64_t,
   avoiding raw memory operations on (possibly non-trivial) seq_t. */
static uint64_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0;

seq_t a(idx_t n) {
    if (n > cache_len) {
        idx_t new_len = n < 128 ? 128 : n + n / 2;
        uint64_t *nc = (uint64_t *)realloc(cache, (size_t)new_len * sizeof(uint64_t));
        if (!nc) return (seq_t)0;
        cache = nc;
        cache_len = new_len;
    }
    for (idx_t i = computed + 1; i <= n; i++) {
        if (i <= 3) {
            cache[i - 1] = 1;
        } else {
            uint64_t t = cache[i - 4];          /* a(i-3) */
            cache[i - 1] = cache[t - 1] + cache[i - t - 1];
        }
        computed = i;
    }
    return (seq_t)cache[n - 1];
}
