/*
 * A237631  Number of (n+1) X (2+1) 0..2 arrays with the upper median of every
 *          2 X 2 subblock differing from its horizontal and vertical
 *          neighbors by exactly one.
 * https://oeis.org/A237631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) + 10*a(n-4) - 12*a(n-7) - 6*a(n-8) for n>12
 *            Initial values: a(1)=260, a(2)=636, a(3)=1220, a(4)=2356,
 *            a(5)=4500, a(6)=8594, a(7)=16448, a(8)=31672, a(9)=60362,
 *            a(10)=116180, a(11)=223768, a(12)=429680
 * Range:     u8 none, u16 n <= 9, u32 n <= 26, u64 n <= 60, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static bool initialized = false;

static void init_cache(void) {
    const seq_t base[13] = {0,
        260, 636, 1220, 2356, 4500, 8594, 16448, 31672,
        60362, 116180, 223768, 429680};
    cache_size = 13;          /* indices 0 .. 12 are present */
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache) return;
    for (idx_t i = 0; i < cache_size; ++i) {
        cache[i] = base[i];
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_cache();
    if (n >= cache_size) {
        idx_t new_size = n + 1;                /* need indices 0 .. n */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * cache[i-3] + (seq_t)10 * cache[i-4];
            seq_t t2 = (seq_t)12 * cache[i-7];
            seq_t t3 = (seq_t)6 * cache[i-8];
            cache[i] = t1 - t2 - t3;
        }
        cache_size = new_size;
    }
    return cache[n];
}
