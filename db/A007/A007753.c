/*
 * A007753  a(n) = Sum_{k=0..n-1} binomial(a(k)^2, k).
 * https://oeis.org/A007753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(a(k)^2, k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_done = 0;

/* binomial(m, k) computed multiplicatively: C(m,i+1) = C(m,i)*(m-i)/(i+1) */
static seq_t binom_big(seq_t m, idx_t k) {
    seq_t r = 1;
    for (idx_t i = 0; i < k; i++) {
        if (r == 0) break;
        r = r * (m - (seq_t)i) / ((seq_t)i + 1);
    }
    return r;
}

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 8;
        cache = alloc_zero(cache_cap);
        cache[0] = 1;
        cache_done = 1;
    }
    if (n >= cache_cap) {
        idx_t newcap = cache_cap;
        while (newcap <= n) newcap *= 2;
        cache = grow_zero(cache, cache_cap, newcap);
        cache_cap = newcap;
    }
    while (cache_done <= n) {
        idx_t m = cache_done;
        seq_t sum = 0;
        for (idx_t k = 0; k < m; k++) {
            seq_t sq = cache[k] * cache[k];
            sum += binom_big(sq, k);
        }
        cache[m] = sum;
        cache_done++;
    }
    return cache[n];
}
