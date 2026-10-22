/*
 * A002040  Related to partitions.
 * https://oeis.org/A002040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. 1/f'(q), where f(q) = Sum_{k in Z} (-1)^k*(-q)^(k*(3k-1)/2)
 *            = 1 + q - q^2 - q^5 - q^7 - q^12 + q^15 + ... so f'(q) = Sum_{g
 *            pentagonal} c(g)*g*q^(g-1) with c(g) = (-1)^(k+g), g =
 *            k*(3k-1)/2, k in Z. Recurrence: a(0) = 1, a(m) = -Sum_{g>=2}
 *            c(g)*g*a(m-g+1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a2040_cache = NULL;
static idx_t a2040_len = 0;

static void a2040_extend(idx_t newlen) {
    seq_t *buf = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
    memset((void *)buf, 0, (size_t)newlen * sizeof(seq_t));
    for (idx_t i = 0; i < a2040_len; i++) buf[i] = a2040_cache[i];
    if (a2040_cache) free((void *)a2040_cache);
    a2040_cache = buf;
    for (idx_t m = a2040_len; m < newlen; m++) {
        if (m == 0) { a2040_cache[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m + 1) break;
            idx_t gs[2];
            gs[0] = g1;
            gs[1] = k * (3 * k + 1) / 2;
            for (int t = 0; t < 2; t++) {
                idx_t g = gs[t];
                if (g < 2 || g - 1 > m) continue;
                idx_t j = g - 1;
                seq_t term = (seq_t)g * a2040_cache[m - j];
                if ((k + g) & 1) pos += term;  /* coefficient -g */
                else neg += term;              /* coefficient +g */
            }
        }
        a2040_cache[m] = pos - neg;
    }
    a2040_len = newlen;
}

seq_t a(idx_t n) {
    if (n >= a2040_len) {
        idx_t newlen = n + 1;
        if (newlen < 2 * a2040_len) newlen = 2 * a2040_len;
        a2040_extend(newlen);
    }
    return a2040_cache[n];
}
