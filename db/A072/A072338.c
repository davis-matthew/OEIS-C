/*
 * A072338  EULER transform of A002620 (with the initial 0,0,1 omitted).
 * https://oeis.org/A072338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of b(k) = floor((k+2)^2/4), k>=1 (b =
 *            2,4,6,9,12,...): a(0) = 1, a(n) = (1/n) * Sum_{k=1..n}
 *            c(k)*a(n-k), c(k) = Sum_{d|k} d*b(d)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A072338_c = 0;
static seq_t *A072338_v = 0;
static size_t A072338_cnt = 0;
static size_t A072338_cap = 0;

static seq_t A072338_b(size_t k) {
    seq_t s = (seq_t)k + 2;
    return (s >> 1) * ((s + 1) >> 1);
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    if (A072338_cap == 0) {
        A072338_cap = 64;
        A072338_c = mem_new(seq_t, A072338_cap);
        A072338_v = mem_new(seq_t, A072338_cap);
    }
    if (N >= A072338_cap) {
        size_t nc = A072338_cap;
        while (nc <= N) nc *= 2;
        A072338_c = mem_resize(seq_t, A072338_c, nc);
        A072338_v = mem_resize(seq_t, A072338_v, nc);
        A072338_cap = nc;
    }
    if (A072338_cnt == 0) {
        A072338_v[0] = 1;
        A072338_c[0] = 0;
        A072338_cnt = 1;
    }
    while (A072338_cnt <= N) {
        size_t m = A072338_cnt;
        seq_t ck = 0;
        for (size_t d = 1; d <= m; d++) {
            if (m % d == 0) ck += (seq_t)d * A072338_b(d);
        }
        A072338_c[m] = ck;
        seq_t s = 0;
        for (size_t k = 1; k <= m; k++) {
            s += A072338_c[k] * A072338_v[m - k];
        }
        A072338_v[m] = s / (seq_t)m;
        A072338_cnt++;
    }
    return A072338_v[N];
}
