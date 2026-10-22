/*
 * A003373  Numbers that are the sum of 6 positive 7th powers.
 * https://oeis.org/A003373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate all sums of 6 positive 7th powers with bases 1..15, sort, and return the n-th smallest */

#include <stdlib.h>
#include "../utils/power7.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static int cache_initialized = 0;

static int compare_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void build_cache(void) {
    if (cache_initialized) return;
    cache_initialized = 1;
    
    idx_t max_base = 15;
    idx_t cap = 50000;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    cache_len = 0;
    
    for (idx_t a = 1; a <= max_base; a++) {
        seq_t pa = power7((seq_t)a);
        for (idx_t b = a; b <= max_base; b++) {
            seq_t pb = power7((seq_t)b);
            for (idx_t c = b; c <= max_base; c++) {
                seq_t pc = power7((seq_t)c);
                for (idx_t d = c; d <= max_base; d++) {
                    seq_t pd = power7((seq_t)d);
                    for (idx_t e = d; e <= max_base; e++) {
                        seq_t pe = power7((seq_t)e);
                        for (idx_t f = e; f <= max_base; f++) {
                            seq_t pf = power7((seq_t)f);
                            seq_t sum = pa + pb + pc + pd + pe + pf;
                            
                            if (cache_len >= cap) {
                                cap *= 2;
                                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                            }
                            cache[cache_len++] = sum;
                        }
                    }
                }
            }
        }
    }
    
    qsort(cache, cache_len, sizeof(seq_t), compare_seq_t);
    
    idx_t out = 0;
    for (idx_t i = 0; i < cache_len; i++) {
        if (i == 0 || cache[i] != cache[out - 1]) {
            cache[out++] = cache[i];
        }
    }
    cache_len = out;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    build_cache();
    if (n <= cache_len) {
        return cache[n - 1];
    }
    return 0;
}
