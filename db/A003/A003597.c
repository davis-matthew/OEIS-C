/*
 * A003597  Numbers of the form 3^i*11^j.
 * https://oeis.org/A003597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number of the form 3^i * 11^j, i,j >= 0
 * Range:     u8 n <= 9, u16 n <= 30, u32 n <= 109, u64 n <= 405,
 *            u128 n <= 1554, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t idx3 = 0;
static idx_t idx11 = 0;

static void ensure_cache(idx_t needed) {
    if (cache == NULL) {
        cache_cap = (needed < 64) ? 64 : needed * 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
        idx3 = 0;
        idx11 = 0;
    }
    
    while (cache_len < needed) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        
        seq_t cand3 = cache[idx3] * 3;
        seq_t cand11 = cache[idx11] * 11;
        
        seq_t next;
        if (cand3 <= cand11) {
            next = cand3;
            idx3++;
            if (cand3 == cand11) idx11++;
        } else {
            next = cand11;
            idx11++;
        }
        
        cache[cache_len++] = next;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n - 1];
}
