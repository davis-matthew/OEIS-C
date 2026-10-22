/*
 * A007369  Numbers n such that sigma(x) = n has no solution.
 * https://oeis.org/A007369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: compute sigma(x) for all x up to B = 2n + sqrt(8n) + 2 (since a(n) < 2n + sqrt(8n)),
   mark every value sigma(x) <= B as touched, and return the n-th untouched number. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;   /* untouched numbers found so far */
static uint64_t cache_len = 0;
static uint64_t cache_bound = 0;

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static void build(uint64_t B) {
    uint64_t *sig = (uint64_t *)calloc((size_t)(B + 1), sizeof(uint64_t));
    bool *touched = (bool *)calloc((size_t)(B + 1), sizeof(bool));
    uint64_t d, j, i, cnt = 0;
    if (!sig || !touched) { free(sig); free(touched); return; }
    for (d = 1; d <= B; d++)
        for (j = d; j <= B; j += d)
            sig[j] += d;
    for (i = 1; i <= B; i++)
        if (sig[i] <= B) touched[sig[i]] = true;
    for (i = 1; i <= B; i++)
        if (!touched[i]) cnt++;
    free(cache);
    cache = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cache_len = 0;
    if (cache) {
        for (i = 1; i <= B; i++)
            if (!touched[i]) cache[cache_len++] = i;
        cache_bound = B;
    } else {
        cache_bound = 0;
    }
    free(sig);
    free(touched);
}

seq_t a(idx_t n) {
    if (n > cache_len) {
        uint64_t B = 2 * (uint64_t)n + isqrt_u64(8 * (uint64_t)n) + 2;
        if (B < 2 * cache_bound) B = 2 * cache_bound;
        build(B);
        if (n > cache_len) return (seq_t)0;
    }
    return (seq_t)cache[n - 1];
}
