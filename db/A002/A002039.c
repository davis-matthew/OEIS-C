/*
 * A002039  Convolution inverse of A143348.
 * https://oeis.org/A002039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k+1) * sigma(k+1) * a(n-k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed = 0;
static uint64_t *sig = NULL;
static idx_t sig_len = 0;

static void build_sigma(idx_t m) {
    idx_t i, j;
    if (sig != NULL && sig_len > m) return;
    free(sig);
    sig = (uint64_t *)calloc((size_t)(m + 1), sizeof(uint64_t));
    for (i = 1; i <= m; i++)
        for (j = i; j <= m; j += i)
            sig[j] += i;
    sig_len = m + 1;
}

seq_t a(idx_t n) {
    idx_t i, k;
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = alloc_zero(cache_cap);
        cache[0] = 1;
        computed = 1;
    } else if (cache_cap <= n) {
        cache = grow_zero(cache, cache_cap, n + 1);
        cache_cap = n + 1;
    }
    if (n < computed) return cache[n];

    build_sigma(n + 1);
    for (i = computed; i <= n; i++) {
        seq_t pos = 0, neg = 0;
        for (k = 1; k <= i; k++) {
            seq_t term = cache[i - k] * (seq_t)sig[k + 1];
            if (k & 1) pos += term;
            else neg += term;
        }
        cache[i] = pos - neg;
    }
    computed = n + 1;
    return cache[n];
}
