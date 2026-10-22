/*
 * A002132  Generalized sum of divisors function.
 * https://oeis.org/A002132
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(q) = N(q)/D(q) with N(q) = (1/2)*Sum_{k>=2}
 *            (-1)^k*k*binomial(k+1,3)*q^(k^2), D(q) = 1 + 2*Sum_{k>=1}
 *            (-q)^(k^2); hence a(n) = [n=k^2]*(-1)^k*k^2*(k^2-1)/12 +
 *            2*Sum_{k>=1} (-1)^(k+1)*a(n-k^2), a(n) = 0 for n < 4
 * Range:     u8 n <= 19, u16 n <= 116, u32 n <= 4619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *a002132_cache = NULL;
static idx_t a002132_len = 0;

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n >= a002132_len) {
        idx_t newlen = n + 1;
        seq_t *nv = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
        if (nv == NULL) return 0;
        for (idx_t i = 0; i < newlen; i++) nv[i] = 0;
        for (idx_t i = 0; i < a002132_len; i++) nv[i] = a002132_cache[i];
        idx_t start = a002132_len < 4 ? 4 : a002132_len;
        free(a002132_cache);
        a002132_cache = nv;
        for (idx_t i = start; i < newlen; i++) {
            seq_t pos = 0, neg = 0;
            idx_t k;
            for (k = 1; k * k <= i; k++) {
                if (k * k == i && k >= 2) {
                    uint64_t m = (uint64_t)k * k;
                    uint64_t t = m * (m - 1) / 12;
                    if (k % 2 == 0) pos += (seq_t)t;
                    else neg += (seq_t)t;
                }
                if (k % 2 == 1) pos += 2 * nv[i - k * k];
                else neg += 2 * nv[i - k * k];
            }
            nv[i] = pos - neg;
        }
        a002132_len = newlen;
    }
    return a002132_cache[n];
}
