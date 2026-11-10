/*
 * A071019  Euler transform of A002487.
 * https://oeis.org/A071019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+1), where b is the Euler transform with b(0)=1, b(m)
 *            = (1/m)*Sum_{k=1..m} c(k)*b(m-k), c(k) = Sum_{d|k} d*s(d+1), s =
 *            A002487 (s(0)=0, s(1)=1, s(2n)=s(n), s(2n+1)=s(n)+s(n+1))
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n + 1;
    seq_t *s = mem_new(seq_t, N + 3);
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    size_t i, d, k, m;
    s[0] = 0;
    s[1] = 1;
    for (i = 2; i <= N + 1; i++) {
        if (i % 2 == 0) s[i] = s[i / 2];
        else s[i] = s[i / 2] + s[i / 2 + 1];
    }
    for (d = 1; d <= N; d++) {
        seq_t v = (seq_t)d * s[d + 1];
        for (k = d; k <= N; k += d) c[k] = c[k] + v;
    }
    b[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t sum = 0;
        for (k = 1; k <= m; k++) sum = sum + c[k] * b[m - k];
        b[m] = sum / (seq_t)m;
    }
    seq_t r = b[N];
    mem_free(s);
    mem_free(c);
    mem_free(b);
    return r;
}
