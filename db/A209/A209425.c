/*
 * A209425  Column 2 of triangle A209424.
 * https://oeis.org/A209425
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,2) where T(r,c) = [x^r y^c] exp( Sum_{m>=1} x^m/m *
 *            Sum_{k=0..m} binomial(m,k)^m y^k ); T(0,0) = 1, T(r,c) = (1/r) *
 *            Sum_{m=1..r} Sum_{l=0..c} T(r-m,c-l) * binomial(m,l)^m, c = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 16 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A209425_pw(seq_t base, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r = r * base;
        base = base * base;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *t[3];
    seq_t *w[3];
    int j;
    for (j = 0; j < 3; j++) {
        t[j] = mem_new(seq_t, N + 1);
        w[j] = mem_new(seq_t, N + 1);
    }
    for (size_t m = 1; m <= N; m++) {
        seq_t mm = (seq_t)m;
        w[0][m] = 1;
        w[1][m] = A209425_pw(mm, (idx_t)m);
        w[2][m] = (m >= 2) ? A209425_pw(mm * (mm - 1) / 2, (idx_t)m) : (seq_t)0;
    }
    t[0][0] = 1;
    for (size_t r = 1; r <= N; r++) {
        for (int c = 0; c < 3; c++) {
            seq_t s = 0;
            if ((size_t)c > r) { t[c][r] = 0; continue; }
            for (size_t m = 1; m <= r; m++) {
                size_t rem = r - m;
                for (int l = 0; l <= c; l++) {
                    int cc = c - l;
                    if ((size_t)cc > rem) continue;
                    if (l > (int)m) continue;
                    s += t[cc][rem] * w[l][m];
                }
            }
            t[c][r] = s / (seq_t)r;
        }
    }
    seq_t res = t[2][N];
    for (j = 0; j < 3; j++) {
        mem_free(t[j]);
        mem_free(w[j]);
    }
    return res;
}
