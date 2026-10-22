/*
 * A007165  Number of P-graphs with 2n edges.
 * https://oeis.org/A007165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] A(x), where A(x) = E(x) + O(x) is split into
 *            even and odd parts. (5) gives O = x*(E^2 - O^2) = x*A(x)*A(-x);
 *            (3) gives E = 1 + x*O*(1 + E). With c_k = a(k+1) and c_0 = 1:
 *            c_m = Sum_{i+j=m-1} (-1)^j c_i c_j for odd m, c_m = c_{m-1} +
 *            Sum_{i odd, i+j=m-1} c_i c_j for even m >= 2
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n <= cache_len) return cache[n - 1];
    idx_t newlen = n;
    if (newlen < 2 * cache_len) newlen = 2 * cache_len;
    cache = grow_zero(cache, cache_len, newlen);
    idx_t start = cache_len;
    if (start == 0) {
        cache[0] = 1;
        start = 1;
    }
    for (idx_t m = start; m < newlen; m++) {
        idx_t s = m - 1;
        if (m & 1) {
            /* s even: pairs (i, s-i) with i even add, with i odd subtract */
            seq_t pos = 0, neg = 0;
            for (idx_t i = 0; i <= s; i++) {
                seq_t t = cache[i] * cache[s - i];
                if (i & 1) neg += t; else pos += t;
            }
            cache[m] = pos - neg;
        } else {
            seq_t v = cache[m - 1];
            for (idx_t i = 1; i <= s; i += 2) {
                v += cache[i] * cache[s - i];
            }
            cache[m] = v;
        }
    }
    cache_len = newlen;
    return cache[n - 1];
}
