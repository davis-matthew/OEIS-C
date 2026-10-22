/*
 * A005467  6 X 6 stochastic matrices of integers.
 * https://oeis.org/A005467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   with H(r) = number of 6 X 6 nonnegative integer matrices with
 *            all row and column sums r, H(r) = Sum_{i=0..10}
 *            a(i)*binomial(r+5+i, 5+2i); so a(r) = H(r) - Sum_{i<r}
 *            a(i)*binomial(r+5+i, 5+2i), a(0) = 1, a(n) = 0 for n > 10. H(r)
 *            is computed by meet-in-the-middle over 3-row halves, using
 *            states of...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   binom_u64, sort6, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/binom_u64.h"
#include "../utils/sort6.h"

static int R, Bs;
static unsigned char *comp;
static int ncomp;
static uint32_t *m2;
static uint64_t *m3;

static uint32_t code6(const int *v)
{
    uint32_t c = 0;
    int j;
    for (j = 5; j >= 0; j--) c = c * (uint32_t)Bs + (uint32_t)v[j];
    return c;
}

/* number of rows (sum R) fitting in remaining column sums c */
static uint32_t f2(const int *c)
{
    uint32_t k = code6(c);
    uint64_t w[16], nw[16];
    int j, s, t;
    if (m2[k] != UINT32_MAX) return m2[k];
    for (s = 0; s <= R; s++) w[s] = 0;
    w[0] = 1;
    for (j = 0; j < 6; j++) {
        int lim = R - c[j];
        for (s = 0; s <= R; s++) {
            uint64_t acc = 0;
            for (t = 0; t <= lim && t <= s; t++) acc += w[s - t];
            nw[s] = acc;
        }
        for (s = 0; s <= R; s++) w[s] = nw[s];
    }
    m2[k] = (uint32_t)w[R];
    return m2[k];
}

/* number of 2-row completions-from-above: ways for 3 rows to leave sorted remainder c */
static uint64_t f3(const int *c)
{
    uint32_t k = code6(c);
    uint64_t total = 0;
    int i, j, d[6];
    bool ok;
    if (m3[k] != UINT64_MAX) return m3[k];
    for (i = 0; i < ncomp; i++) {
        const unsigned char *x = comp + 6 * i;
        ok = true;
        for (j = 0; j < 6; j++) {
            d[j] = c[j] + x[j];
            if (d[j] > R) { ok = false; break; }
        }
        if (!ok) continue;
        sort6(d);
        total += f2(d);
    }
    m3[k] = total;
    return total;
}

static void gen_comp(int pos, int rem, unsigned char *buf)
{
    int v;
    if (pos == 5) {
        buf[5] = (unsigned char)rem;
        memcpy(comp + 6 * ncomp, buf, 6);
        ncomp++;
        return;
    }
    for (v = 0; v <= rem; v++) {
        buf[pos] = (unsigned char)v;
        gen_comp(pos + 1, rem - v, buf);
    }
}

static void gen_c(int pos, int lo, int rem, int *c, uint64_t *H)
{
    int v;
    if (pos == 6) {
        if (rem == 0) {
            static const uint64_t fact[7] = {1, 1, 2, 6, 24, 120, 720};
            uint64_t mult = 720;
            int i = 0, j, e[6];
            while (i < 6) {
                j = i;
                while (j < 6 && c[j] == c[i]) j++;
                mult /= fact[j - i];
                i = j;
            }
            for (j = 0; j < 6; j++) e[j] = R - c[5 - j];
            *H += mult * f3(c) * f3(e);
        }
        return;
    }
    for (v = lo; v <= R; v++) {
        if (v * (6 - pos) > rem) break;
        if (rem - v > (5 - pos) * R) continue;
        c[pos] = v;
        gen_c(pos + 1, v, rem - v, c, H);
    }
}

static uint64_t count_H(int r)
{
    size_t size, i;
    uint64_t H = 0;
    int c[6];
    unsigned char buf[6];
    if (r == 0) return 1;
    R = r;
    Bs = r + 1;
    size = 1;
    for (i = 0; i < 6; i++) size *= (size_t)Bs;
    m2 = (uint32_t *)malloc(size * sizeof(uint32_t));
    m3 = (uint64_t *)malloc(size * sizeof(uint64_t));
    memset(m2, 0xFF, size * sizeof(uint32_t));
    memset(m3, 0xFF, size * sizeof(uint64_t));
    comp = (unsigned char *)malloc(6 * 3003 + 6);
    ncomp = 0;
    gen_comp(0, r, buf);
    gen_c(0, 0, 3 * r, c, &H);
    free(m2);
    free(m3);
    free(comp);
    return H;
}

seq_t a(idx_t n)
{
    static bool done = false;
    static uint64_t coef[11];
    if (n > 10) return (seq_t)0;
    if (!done) {
        int r, i;
        for (r = 0; r <= 10; r++) {
            uint64_t v = count_H(r);
            for (i = 0; i < r; i++) v -= coef[i] * binom_u64(r + 5 + i, 5 + 2 * i);
            coef[r] = v;
        }
        done = true;
    }
    return u128_to_seq((unsigned __int128)coef[n]);
}
