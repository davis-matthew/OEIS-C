/*
 * A005362  Hoggatt sequence with parameter d=4.
 * https://oeis.org/A005362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 12*C(n,k)*C(n+1,k)*C(n+2,k)*C(n+3,k) /
 *            ((k+1)^3*(k+2)^2*(k+3)) (terms of
 *            4F3([-3-n,-2-n,-1-n,-n],[2,3,4],1)); for large n the recurrence
 *            (n+3)(n+4)(n+5)(n+6)*a(n) = 6(n+1)(n+3)(n+4)(2n+5)*a(n-1) +
 *            4(n-1)n(4n+7)(4n+9)*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 811 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

/* exact term with all cancellation done on small integers first */
static seq_t a5362_term(idx_t n, idx_t k) {
    idx_t nn = 4 * k + 1;
    uint64_t *num = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nn);
    idx_t c = 0, i, j, t;
    num[c++] = 12;
    for (i = 0; i < 4; i++)
        for (j = 0; j < k; j++)
            num[c++] = (uint64_t)(n + i - j);
    for (t = 0; t < 4 * k + 6; t++) {
        uint64_t d;
        if (t < 4 * k) d = (uint64_t)(t % (k == 0 ? 1 : k)) + 1;
        else {
            idx_t r = t - 4 * k;
            d = (r < 3) ? (uint64_t)(k + 1) : (r < 5) ? (uint64_t)(k + 2) : (uint64_t)(k + 3);
        }
        for (j = 0; j < c && d > 1; j++) {
            uint64_t g = gcd_u64(d, num[j]);
            if (g > 1) { d /= g; num[j] /= g; }
        }
    }
    seq_t r = 1;
    for (j = 0; j < c; j++) r *= (seq_t)num[j];
    free(num);
    return r;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n <= 40) {
        seq_t s = 0;
        idx_t k;
        for (k = 0; k <= n; k++) s += a5362_term(n, k);
        return s;
    }

    if (cache == NULL) {
        cache = grow_zero(NULL, 0, 2);
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
    }
    if (n >= cache_len) {
        idx_t newlen = n + 1;
        cache = grow_zero(cache, cache_len, newlen);
        while (cache_len < newlen) {
            idx_t m = cache_len;
            seq_t num = (seq_t)6 * (seq_t)(m + 1) * (seq_t)(m + 3) * (seq_t)(m + 4) * (seq_t)(2 * m + 5) * cache[m - 1]
                      + (seq_t)4 * (seq_t)(m - 1) * (seq_t)m * (seq_t)(4 * m + 7) * (seq_t)(4 * m + 9) * cache[m - 2];
            seq_t den = (seq_t)(m + 3) * (seq_t)(m + 4) * (seq_t)(m + 5) * (seq_t)(m + 6);
            cache[m] = num / den;
            cache_len = m + 1;
        }
    }
    return cache[n];
}
