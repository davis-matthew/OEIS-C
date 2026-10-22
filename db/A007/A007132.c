/*
 * A007132  Number of unlabeled connected strength 3 Eulerian graphs with n
 *          nodes, 2 of odd degree.
 * https://oeis.org/A007132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n-1)!) * Sum_{sigma in S_{n-1}} 2^(2*N(sigma) +
 *            c(sigma)), where c(sigma) is the number of cycles of sigma, and
 *            N(sigma) = Sum_{cycles k} floor(k/2) + Sum_{pairs of cycles k,l}
 *            gcd(k,l). This counts rooted unlabeled Eulerian multigraphs with
 *            edge multiplicity <= 3 using Burnside's lemm...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_div_small, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_div_small.h"
#include "../utils/gcd_u64.h"

static uint32_t *g_acc;
static size_t g_L;
static uint32_t *g_buf;
static uint64_t *g_pj, *g_pm;
static size_t g_cnt;

static void big_addshift(const uint32_t *b, size_t blen, uint64_t e)
{
    size_t q = (size_t)(e / 32), i;
    unsigned s = (unsigned)(e % 32);
    uint64_t carry = 0;
    for (i = 0; i < blen && q + i < g_L; i++) {
        uint64_t v = (uint64_t)b[i] << s;
        uint64_t t = (uint64_t)g_acc[q + i] + (v & 0xffffffffULL) + carry;
        g_acc[q + i] = (uint32_t)t;
        carry = (t >> 32) + (v >> 32);
    }
    for (i = q + blen; carry && i < g_L; i++) {
        uint64_t t = (uint64_t)g_acc[i] + carry;
        g_acc[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void big_mul_small(uint32_t *b, size_t *len, uint32_t x)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)b[i] * x + carry;
        b[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry && *len < g_L) {
        b[(*len)++] = (uint32_t)carry;
        carry >>= 32;
    }
}

static void dfs(uint64_t k, uint64_t r, uint64_t N, uint64_t c,
                const uint32_t *cls, size_t clen, size_t depth)
{
    size_t i;
    if (r == 0) {
        big_addshift(cls, clen, 2 * N + c);
        return;
    }
    if (k == 1) {
        uint64_t tot = 0, dN;
        for (i = 0; i < g_cnt; i++) tot += g_pm[i];
        dN = r * (r - 1) / 2 + r * tot;
        big_addshift(cls, clen, 2 * (N + dN) + c + r);
        return;
    }
    dfs(k - 1, r, N, c, cls, clen, depth);
    {
        uint64_t G = 0, m;
        uint32_t *B = g_buf + depth * g_L;
        size_t blen = clen;
        for (i = 0; i < g_cnt; i++) G += g_pm[i] * gcd_u64(k, g_pj[i]);
        memcpy(B, cls, clen * sizeof(uint32_t));
        for (m = 1; k * m <= r; m++) {
            uint64_t base = r - k * (m - 1), t, dN, rem, nk;
            for (t = 0; t < k; t++) big_mul_small(B, &blen, (uint32_t)(base - t));
            big_div_small(B, &blen, (uint32_t)(k * m));
            dN = m * (k / 2) + k * (m * (m - 1) / 2) + m * G;
            rem = r - k * m;
            nk = (k - 1 < rem) ? k - 1 : rem;
            g_pj[g_cnt] = k;
            g_pm[g_cnt] = m;
            g_cnt++;
            dfs(nk, rem, N + dN, c + m, B, blen, depth + 1);
            g_cnt--;
        }
    }
}

static uint64_t bitlen_u64(uint64_t x)
{
    uint64_t b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n - 1, fbits = 0, i;
    size_t len;
    uint32_t one[1];
    seq_t res = 0;

    for (i = 2; i <= m; i++) fbits += bitlen_u64(i);
    g_L = (size_t)((m * m + fbits + 2 * bitlen_u64(n) + 64) / 32 + 4);
    g_acc = (uint32_t *)calloc(g_L, sizeof(uint32_t));
    g_buf = (uint32_t *)calloc((size_t)(m + 2) * g_L, sizeof(uint32_t));
    g_pj = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    g_pm = (uint64_t *)malloc((size_t)(m + 2) * sizeof(uint64_t));
    g_cnt = 0;

    one[0] = 1;
    dfs(m, m, 0, 0, one, 1, 0);

    len = g_L;
    while (len > 1 && g_acc[len - 1] == 0) len--;
    for (i = 2; i <= m; i++) big_div_small(g_acc, &len, (uint32_t)i);

    {
        size_t j = len;
        while (j > 0) {
            j--;
            res *= 256; res *= 256; res *= 256; res *= 256;
            res += (seq_t)g_acc[j];
        }
    }

    free(g_acc);
    free(g_buf);
    free(g_pj);
    free(g_pm);
    return res;
}
