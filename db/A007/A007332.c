/*
 * A007332  Expansion of 6-dimensional cusp form (eta(q) * eta(q^3))^6 in
 *          powers of q.
 * https://oeis.org/A007332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) = [x^(n-1)] h(x)^2, where h(x) = P(x)^3 * P(x^3)^3 and P(x)
 *            = Product_{k>0} (1 - x^k). Jacobi's identity gives P(x)^3 =
 *            Sum_{j>=0} (-1)^j (2j+1) x^(j(j+1)/2), so h_m = Sum_{j(j+1)/2 +
 *            3k(k+1)/2 = m} (-1)^(j+k) (2j+1)(2k+1). a(0) = 0
 * Range:     i8 n <= 7, i16 n <= 52, i32 n <= 4173, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *h = NULL;
static idx_t hlen = 0;

static void ensure_h(idx_t need) {
    if (hlen >= need + 1) return;
    idx_t newlen = need + 1;
    if (newlen < 2 * hlen) newlen = 2 * hlen;
    if (h != NULL) release(h, hlen);
    h = alloc_zero(newlen);
    hlen = newlen;
    for (idx_t j = 0; j * (j + 1) / 2 < newlen; j++) {
        idx_t tj = j * (j + 1) / 2;
        for (idx_t k = 0; tj + 3 * (k * (k + 1) / 2) < newlen; k++) {
            idx_t m = tj + 3 * (k * (k + 1) / 2);
            seq_t t = (seq_t)(long long)(2 * j + 1);
            t *= (seq_t)(long long)(2 * k + 1);
            if ((j + k) & 1) h[m] -= t;
            else h[m] += t;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t m = n - 1;
    ensure_h(m);
    seq_t s = 0;
    for (idx_t i = 0; i <= m; i++) {
        s += h[i] * h[m - i];
    }
    return s;
}
