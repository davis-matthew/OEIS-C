/*
 * A007096  Expansion of theta_3 / theta_4.
 * https://oeis.org/A007096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = phi(q)/phi(-q), where 1/phi(-q) = Sum P(n) q^n
 *            (overpartitions, A015128): P(0) = 1, P(n) = 2*Sum_{k>=1, k^2<=n}
 *            (-1)^(k+1) P(n-k^2); a(n) = P(n) + 2*Sum_{k>=1, k^2<=n} P(n-k^2)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 74, u64 n <= 255, u128 n <= 925,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *P = NULL;
static idx_t P_len = 0;

static void ensure(idx_t n) {
    if (n < P_len) return;
    idx_t newlen = n + 1;
    if (newlen < 2 * P_len) newlen = 2 * P_len;
    if (P == NULL) P = alloc_zero(newlen);
    else P = grow_zero(P, P_len, newlen);
    for (idx_t i = P_len; i < newlen; i++) {
        if (i == 0) { P[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; k * k <= i; k++) {
            if (k & 1) pos += P[i - k * k];
            else neg += P[i - k * k];
        }
        P[i] = (pos - neg) * 2;
    }
    P_len = newlen;
}

seq_t a(idx_t n) {
    ensure(n);
    seq_t r = P[n];
    for (idx_t k = 1; k * k <= n; k++) {
        r += P[n - k * k] * 2;
    }
    return r;
}
