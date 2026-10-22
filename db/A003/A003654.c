/*
 * A003654  Squarefree integers m such that the fundamental unit of Q(sqrt(m))
 *          has norm -1. Also, squarefree integers m such that the Pell
 *          equation x^2 - m*y^2 = -1 is soluble.
 * https://oeis.org/A003654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 7280, u32 n <= 9446, u64 n <= 9446,
 *            u128 n <= 9446, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"

/* Find squarefree m where x^2 - m*y^2 = -1 has a solution (cf period of sqrt(m) is odd) */

static bool is_squarefree(uint64_t n) {
    if (n <= 1) return n == 1;
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % (i * i) == 0) return false;
    }
    return true;
}

static int cf_period(uint64_t D) {
    uint64_t a0 = a000037_isqrt(D);
    if (a0 * a0 == D) return 0;
    
    uint64_t m = 0, d = 1, a = a0;
    int period = 0;
    
    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;
        if (a == 2 * a0) break;
    }
    
    return period;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cached_up_to = 0;

seq_t a(idx_t n) {
    if (cache == NULL || n > cache_len) {
        idx_t new_len = (n > cache_len) ? n * 2 : 100;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 0;
        }
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n <= cached_up_to) {
        return cache[n - 1];
    }
    
    seq_t count = cached_up_to;
    seq_t m = (cached_up_to > 0) ? cache[cached_up_to - 1] : 1;
    
    while (count < (seq_t)n) {
        m++;
        if (is_squarefree((uint64_t)m) && cf_period((uint64_t)m) % 2 == 1) {
            count++;
            cache[(idx_t)(count - 1)] = m;
        }
    }
    
    cached_up_to = n;
    return cache[n - 1];
}
