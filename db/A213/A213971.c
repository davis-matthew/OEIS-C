/*
 * A213971  List of primitive words over the alphabet {2,3}.
 * https://oeis.org/A213971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th primitive word over alphabet {2,3} in lexicographic
 *            order, interpreted as a decimal integer
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_primitive_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_primitive_bits.h"

/* cache of already generated primitive words */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static size_t next_len = 1;   /* next word length to generate */

/* ensure the cache has room for at least new_cap elements */
static void ensure_cache_capacity(size_t new_cap) {
    if (new_cap <= cache_capacity)
        return;
    size_t cap = cache_capacity ? cache_capacity * 2 : 16;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_capacity = cap;
}

/* generate all primitive words of length L and append to the cache */
static void generate_length(size_t L) {
    uint64_t total = 1ULL << L;  /* 2^L words – safe for L <= 63 */
    for (uint64_t num = 0; num < total; ++num) {
        if (!is_primitive_bits(num, L))
            continue;
        seq_t val = 0;
        for (size_t pos = 0; pos < L; ++pos) {
            uint64_t bit = (num >> (L - 1 - pos)) & 1;
            val = val * 10 + (bit ? 3 : 2);
        }
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = val;
    }
}

/* required externally linked function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;            /* undefined for n < 1, but safe */
    idx_t target = n - 1;     /* zero‑based index */
    while (cache_size <= (size_t)target) {
        generate_length(next_len);
        ++next_len;
    }
    return cache[(size_t)target];
}
