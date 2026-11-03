/*
 * A002619  Number of 2-colored patterns on an n X n board.
 * https://oeis.org/A002619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n^2)*Sum_{d|n} phi(d)^2*(n/d)!*d^(n/d)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 97 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* value = Q*N + R, with 0 <= R < N; multiply by small c */
static void a2619_mul(seq_t *Q, uint64_t *R, uint64_t c, uint64_t N) {
    unsigned __int128 t = (unsigned __int128)(*R) * c;
    *Q = *Q * (seq_t)c + (seq_t)(uint64_t)(t / N);
    *R = (uint64_t)(t % N);
}

static void a2619_term(idx_t n, idx_t d, uint64_t N, seq_t *QS, uint64_t *RS) {
    idx_t m = n / d, i;
    uint64_t ph = totient_u64(d);
    seq_t Q = 0;
    uint64_t R = 1 % N;
    if (N == 1) Q = 1;
    a2619_mul(&Q, &R, ph, N);
    a2619_mul(&Q, &R, ph, N);
    for (i = 2; i <= m; i++) a2619_mul(&Q, &R, i, N);
    for (i = 0; i < m; i++) a2619_mul(&Q, &R, d, N);
    *QS += Q;
    *RS += R;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n * (uint64_t)n;
    seq_t QS = 0;
    uint64_t RS = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a2619_term(n, d, N, &QS, &RS);
            if (d * d != n) a2619_term(n, n / d, N, &QS, &RS);
        }
    }
    return QS + (seq_t)(RS / N);
}
