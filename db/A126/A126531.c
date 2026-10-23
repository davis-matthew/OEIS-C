/*
 * A126531  Number of base 10 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*a(n-1) + 3*a(n-2) - 9*a(n-3) - a(n-4) + 2*a(n-5);
 *            Initial values: a(0)=1, a(1)=10, a(2)=80, a(3)=660, a(4)=5430
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void cache_grow(idx_t max_index) {
    if (max_index < cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap <= (size_t)max_index) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < cache_size) return cache[n];
    cache_grow(n);
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)           cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)10;
        else if (i == 2)      cache[i] = (seq_t)80;
        else if (i == 3)      cache[i] = (seq_t)660;
        else if (i == 4)      cache[i] = (seq_t)5430;
        else {
            seq_t t1 = (seq_t)8 * cache[i-1];
            seq_t t2 = (seq_t)3 * cache[i-2];
            seq_t t3 = (seq_t)9 * cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = (seq_t)2 * cache[i-5];
            cache[i] = t1 + t2 - t3 - t4 + t5;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
