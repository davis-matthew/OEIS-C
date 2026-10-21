/*
 * A077077  Trajectory of 775 under the Reverse and Add! operation carried out
 *          in base 2, written in base 10.
 * https://oeis.org/A077077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 40, u64 n <= 104, u128 n <= 232,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A077077: Reverse‑and‑Add in base 2 starting from 775.
 * a(0) = 775.
 * a(n+1) = a(n) + rev_bits(a(n)),
 * where rev_bits(x) reverses the binary representation of x
 * (leading zeros are omitted).
 * The implementation builds the terms on demand and caches them,
 * so subsequent calls with any index order run in amortised O(1).
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t reverse_bits(seq_t x) {
    seq_t rev = 0;
    while (x != 0) {
        rev = (rev << 1) | (x & 1);
        x = x >> 1;
    }
    return rev;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t n) {
    if (n < cache_len) return;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
    if (cache_len == 0) {
        cache[0] = (seq_t)775;
        cache_len = 1;
    }
}

seq_t a(idx_t n) {
    ensure_cache((size_t)n);
    while (cache_len <= (size_t)n) {
        seq_t last = cache[cache_len - 1];
        seq_t rev  = reverse_bits(last);
        cache[cache_len] = last + rev;
        cache_len++;
    }
    return cache[(size_t)n];
}
