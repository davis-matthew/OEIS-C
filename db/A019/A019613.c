/*
 * A019613  Expansion of 1/((1-4*x)*(1-7*x)*(1-9*x)).
 * https://oeis.org/A019613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 127*a(n-2) + 252*a(n-3), with a(0)=1,
 *            a(1)=20, a(2)=273
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Compute the n-th term of OEIS sequence A019613 (1/((1-4x)(1-7x)(1-9x))) */
seq_t a(idx_t n) {
    /* Handle the first three terms directly */
    if (n <= 2) {
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)20;
            case 2: return (seq_t)273;
        }
    }

    /* Initialize cache with the base terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)273;
        cache_size = 3;          /* cache holds indices 0..2 */
    }

    /* Extend cache if needed */
    if (cache_size <= (size_t)n) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)20 * cache[i-1]
                      - (seq_t)127 * cache[i-2]
                      + (seq_t)252 * cache[i-3];
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
