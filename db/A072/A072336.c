/*
 * A072336  EULER transform of A064831 (with its initial 1 omitted).
 * https://oeis.org/A072336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} c(k)*a(n-k), a(0) = 1, where c(k) =
 *            Sum_{d|k} d*b(d) and b(d) = A064831(d+1), A064831(m) = F(m+1)^2
 *            - [m even] (Euler transform)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A072336_b(const seq_t *f, idx_t d)
{
    /* A064831(d+1) = F(d+2)^2 - (d+1 even ? 1 : 0) */
    seq_t sq = f[d + 2] * f[d + 2];
    if (((d + 1) & 1) == 0) return sq - 1;
    return sq;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 3);
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);
    f[0] = 0;
    f[1] = 1;
    for (size_t i = 2; i <= N + 2; ++i) f[i] = f[i - 1] + f[i - 2];
    for (size_t d = 1; d <= N; ++d) {
        seq_t v = (seq_t)d * A072336_b(f, (idx_t)d);
        for (size_t m = d; m <= N; m += d) c[m] += v;
    }
    r[0] = 1;
    for (size_t m = 1; m <= N; ++m) {
        seq_t s = 0;
        for (size_t k = 1; k <= m; ++k) s += c[k] * r[m - k];
        r[m] = s / (seq_t)m;
    }
    seq_t res = r[N];
    mem_free(f);
    mem_free(c);
    mem_free(r);
    return res;
}
