/*
 * A115821  Integers i such that 6*i XOR 15*i = 21*i.
 * https://oeis.org/A115821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth non‑negative integer i such that (6*i & 15*i) ==
 *            0
 * Range:     u8 n <= 27, u16 n <= 913, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_i = 0;

    while (cache_size <= n) {
        unsigned long long val = next_i;
        unsigned long long v6 = val * 6ULL;
        unsigned long long v15 = val * 15ULL;
        if ((v6 & v15) == 0ULL) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)val;
        }
        ++next_i;
    }

    return cache[n];
}
