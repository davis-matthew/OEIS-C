/*
 * A004701  Expansion of e.g.f. 1/(4 - exp(x) - exp(2*x) - exp(3*x)).
 * https://oeis.org/A004701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (1 + 2^k + 3^k) *
 *            a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static seq_t compute_power(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
        cache[0] = 1;
    } else if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memcpy(new_cache, cache, old_len * sizeof(seq_t));
        memset(new_cache + old_len, 0, (cache_len - old_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    seq_t result = 0;
    seq_t binom_coef = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        binom_coef = binom_coef * (seq_t)(n - k + 1) / (seq_t)k;
        seq_t coef = (seq_t)1 + compute_power((seq_t)2, k) + compute_power((seq_t)3, k);
        result += binom_coef * coef * a(n - k);
    }
    
    cache[n] = result;
    return result;
}
