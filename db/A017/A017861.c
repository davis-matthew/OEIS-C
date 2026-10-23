/*
 * A017861  Expansion of 1/(1-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 125, u64 n <= 239,
 *            u128 n <= 467, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12) for n>11
 * Initial values: a(0)=1, a(1..6)=0, a(7..12)=1
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t idx = (size_t)n;

    if (cache_size == 0) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { /* handle allocation failure if desired */ }
        cache[0] = (seq_t)1;
        for (size_t i = 1; i < 7; ++i) cache[i] = (seq_t)0;
        for (size_t i = 7; i < 13; ++i) cache[i] = (seq_t)1;
    }

    if (idx < cache_size)
        return cache[idx];

    size_t new_size = idx + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) { /* handle allocation failure if desired */ }
    cache = new_cache;

    for (size_t i = cache_size; i < new_size; ++i) {
        seq_t sum = (seq_t)0;
        if (i >= 7)  sum += cache[i - 7];
        if (i >= 8)  sum += cache[i - 8];
        if (i >= 9)  sum += cache[i - 9];
        if (i >= 10) sum += cache[i - 10];
        if (i >= 11) sum += cache[i - 11];
        if (i >= 12) sum += cache[i - 12];
        cache[i] = sum;
    }
    cache_size = new_size;
    return cache[idx];
}
