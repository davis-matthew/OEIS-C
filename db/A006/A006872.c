/*
 * A006872  Numbers k such that phi(k) = phi(sigma(k)).
 * https://oeis.org/A006872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer k such that phi(k) =
 *            phi(sigma(k))
 * Range:     u8 n <= 16, u16 n <= 953, u32 n <= 9079, u64 n <= 9079,
 *            u128 n <= 9079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t phi(uint64_t n) {
    if (n == 1) return 1;
    uint64_t result = n;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
        p++;
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

static uint64_t sigma(uint64_t n) {
    uint64_t sum = 0;
    for (uint64_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i) {
                sum += n / i;
            }
        }
    }
    return sum;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t needed) {
    if (needed <= cache_cap) return;
    
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1000;
    while (new_cap < needed) new_cap *= 2;
    
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    ensure_cache(n);
    
    if (cache_len >= n) {
        return cache[n - 1];
    }
    
    uint64_t k = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] + 1 : 1;
    
    while (cache_len < (idx_t)n) {
        uint64_t sig = sigma(k);
        if (phi(k) == phi(sig)) {
            ensure_cache(cache_len + 1);
            cache[cache_len] = (seq_t)k;
            cache_len++;
        }
        k++;
    }
    
    return cache[n - 1];
}
