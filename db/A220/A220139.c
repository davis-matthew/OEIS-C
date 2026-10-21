/*
 * A220139  The highest value of the Collatz iteration (3x+1) starting at
 *          a(n-1) + 1, with a(1) = 1.
 * https://oeis.org/A220139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=max Collatz value starting from a(n-1)+1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   collatz_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/collatz_max.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n) {
    if (n <= cache_len) return;
    size_t new_cap = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!cache) exit(1);
    if (cache_len == 0) {
        cache[1] = 1;
        cache_len = 1;
    }
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t start = cache[i - 1] + 1;
        cache[i] = collatz_max(start);
    }
    cache_len = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 */
    ensure_cache(n);
    return cache[n];
}
