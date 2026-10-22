/*
 * A004985  a(n) = (2^n/n!)*Product_{k=0..n-1} (4*k + 5).
 * https://oeis.org/A004985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4*n+1)*a(n-1)/n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   gcd_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd_val.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 1;
        cache_size = 1;
    }

    while (n >= cache_size) {
        if (cache_size >= cache_capacity) {
            idx_t newcap = cache_capacity * 2;
            seq_t *nc = (seq_t *)malloc(newcap * sizeof(seq_t));
            idx_t j;
            for (j = 0; j < cache_size; j++) nc[j] = cache[j];
            free(cache);
            cache = nc;
            cache_capacity = newcap;
        }

        idx_t i = cache_size;
        seq_t prev = cache[i - 1];
        seq_t si = (seq_t)i;
        /* g = gcd(prev, i); then i/g divides 2*(4i+1) exactly */
        seq_t gs = gcd_val(prev, si);
        idx_t g = (gs == 0) ? 1 : (idx_t)(long long)gs;
        if (g == 0) g = 1;
        idx_t d = i / g;
        if (d == 0) d = 1;
        idx_t m = (2 * (4 * i + 1)) / d;
        cache[i] = (prev / (seq_t)g) * (seq_t)m;
        cache_size++;
    }

    return cache[n];
}
