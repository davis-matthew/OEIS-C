/*
 * A005309  Fermionic string states.
 * https://oeis.org/A005309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. Product_{k>=1} ((1+x^k)/(1-x^k))^(k-1). Arbitrary
 *            precision: a(0) = 1, a(n) = (1/n) * Sum_{m=1..n} b(m)*a(n-m),
 *            where b(m) = Sum_{d|m, m/d odd} 2*d*(d-1) (logarithmic
 *            derivative of the g.f.). Fixed width: the product is expanded
 *            directly, working modulo 2^w, which needs no division
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 44, u64 n <= 107, u128 n <= 274,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *A_cache = NULL;
static uint64_t *B_cache = NULL;
static idx_t cache_len = 0;

static seq_t a_fixed(idx_t n) {
    seq_t *c = alloc_zero(n + 1);
    seq_t r;
    c[0] = 1;
    for (idx_t k = 2; k <= n; k++) {
        for (idx_t e = 0; e + 1 < k; e++) {
            /* multiply by (1 + x^k) */
            for (idx_t i = n; i >= k; i--)
                c[i] += c[i - k];
            /* divide by (1 - x^k) */
            for (idx_t i = k; i <= n; i++)
                c[i] += c[i - k];
        }
    }
    r = c[n];
    release(c, n + 1);
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        /* a(n) needs more than 128 bits once n > 300, so larger n
           cannot be represented in any fixed-width build */
        if (n > 300) return 0;
        return a_fixed(n);
    }
    if (n >= cache_len) {
        idx_t newlen = n + 1;
        if (newlen < 2 * cache_len) newlen = 2 * cache_len;
        A_cache = grow_zero(A_cache, cache_len, newlen);
        B_cache = (uint64_t *)realloc(B_cache, (size_t)newlen * sizeof(uint64_t));
        for (idx_t i = 0; i < newlen; i++) B_cache[i] = 0;
        for (idx_t d = 1; d < newlen; d++) {
            uint64_t w = 2 * (uint64_t)d * (uint64_t)(d - 1);
            if (w == 0) continue;
            for (idx_t m = d; m < newlen; m += 2 * d)
                B_cache[m] += w;
        }
        for (idx_t i = cache_len; i < newlen; i++) {
            if (i == 0) {
                A_cache[0] = 1;
                continue;
            }
            seq_t s = 0;
            for (idx_t m = 1; m <= i; m++) {
                if (B_cache[m] != 0)
                    s += A_cache[i - m] * (idx_t)B_cache[m];
            }
            A_cache[i] = s / i;
        }
        cache_len = newlen;
    }
    return A_cache[n];
}
