/*
 * A001374  Number of relational systems on n nodes. Also number of directed
 *          3-multigraphs with loops on n nodes.
 * https://oeis.org/A001374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n!) * Sum_{partitions L of n} (n!/z(L)) * 4^e(L), e(L)
 *            = Sum_{i,j} gcd(L_i, L_j) (Burnside's lemma: S_n acting on the
 *            n^2 ordered pairs of nodes, loops included, with 4 edge
 *            multiplicities 0..3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   a1374_divs, a1374_gcd, a1374_muls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1374_divs.h"
#include "../utils/a1374_gcd.h"
#include "../utils/a1374_muls.h"

static uint32_t **a1374_buf;
static uint32_t *a1374_S;
static size_t a1374_Lf, a1374_Ls;
static idx_t *a1374_ks, *a1374_ms;

static void a1374_addsh(const uint32_t *x, uint64_t sh)
{
    size_t off = (size_t)(sh / 32), i;
    unsigned b = (unsigned)(sh % 32);
    uint64_t carry = 0;
    for (i = 0; i < a1374_Lf; i++) {
        uint64_t v = (uint64_t)x[i] << b;
        uint64_t cur = (uint64_t)a1374_S[off + i] + (v & 0xffffffffu) + carry;
        a1374_S[off + i] = (uint32_t)cur;
        carry = (cur >> 32) + (v >> 32);
    }
    i = off + a1374_Lf;
    while (carry && i < a1374_Ls) {
        uint64_t cur = (uint64_t)a1374_S[i] + carry;
        a1374_S[i] = (uint32_t)cur;
        carry = cur >> 32;
        i++;
    }
}

static void a1374_rec(idx_t maxk, idx_t rem, int depth, uint64_t e)
{
    idx_t k;
    if (rem == 0) {
        a1374_addsh(a1374_buf[depth], 2 * e);
        return;
    }
    for (k = (maxk < rem ? maxk : rem); k >= 1; k--) {
        uint64_t cross = 0;
        idx_t m;
        int t;
        uint32_t *cb = a1374_buf[depth + 1];
        for (t = 0; t < depth; t++)
            cross += (uint64_t)a1374_ms[t] * a1374_gcd(k, a1374_ks[t]);
        memcpy(cb, a1374_buf[depth], a1374_Lf * sizeof(uint32_t));
        for (m = 1; m * k <= rem; m++) {
            a1374_divs(cb, a1374_Lf, (uint32_t)k);
            a1374_divs(cb, a1374_Lf, (uint32_t)m);
            a1374_ks[depth] = k;
            a1374_ms[depth] = m;
            a1374_rec(k - 1, rem - m * k, depth + 1,
                      e + (uint64_t)m * m * k + 2 * (uint64_t)m * cross);
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t bits = 1;
    idx_t k;
    size_t i;
    seq_t r = 0;
    int s;

    for (k = 2; k <= n; k++) {
        idx_t t = k;
        while (t) { bits++; t >>= 1; }
    }
    a1374_Lf = (size_t)(bits / 32 + 2);
    a1374_Ls = (size_t)((2 * (uint64_t)n * n) / 32) + a1374_Lf + 4;

    a1374_buf = (uint32_t **)malloc((size_t)(n + 2) * sizeof(uint32_t *));
    for (i = 0; i < (size_t)(n + 2); i++)
        a1374_buf[i] = (uint32_t *)calloc(a1374_Lf, sizeof(uint32_t));
    a1374_S = (uint32_t *)calloc(a1374_Ls, sizeof(uint32_t));
    a1374_ks = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    a1374_ms = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));

    a1374_buf[0][0] = 1;
    for (k = 2; k <= n; k++)
        a1374_muls(a1374_buf[0], a1374_Lf, (uint32_t)k);

    a1374_rec(n, n, 0, 0);

    for (k = 2; k <= n; k++)
        a1374_divs(a1374_S, a1374_Ls, (uint32_t)k);

    for (i = a1374_Ls; i-- > 0;) {
        for (s = 24; s >= 0; s -= 8) {
            r *= 256;
            r += (seq_t)((a1374_S[i] >> s) & 255u);
        }
    }

    for (i = 0; i < (size_t)(n + 2); i++)
        free(a1374_buf[i]);
    free(a1374_buf);
    free(a1374_S);
    free(a1374_ks);
    free(a1374_ms);
    return r;
}
