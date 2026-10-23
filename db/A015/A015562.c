/*
 * A015562  Expansion of x/(1 - 7*x - 5*x^2).
 * https://oeis.org/A015562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 5*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t max_computed = 0;

static void ensure_capacity(idx_t needed) {
    if (needed <= cache_capacity) return;
    idx_t new_cap = cache_capacity;
    if (new_cap == 0) new_cap = 2;
    while (needed > new_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) {
        ensure_capacity(1);
        return 0;
    }
    if (n == 1) {
        ensure_capacity(2);
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
        return 1;
    }

    ensure_capacity(n + 1);

    if (max_computed < 1) {
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 7 * cache[i - 1] + 5 * cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
