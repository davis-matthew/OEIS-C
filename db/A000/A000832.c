/*
 * A000832  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over GL_n(2) x S_2 acting on maps F_2^n ->
 *            F_2^2. a(n) = (1/(2*|GL_n(2)|)) * Sum_{g in GL_n(2)} (4^c(g) +
 *            2^o(g)*4^e(g)), where c(g), o(g), e(g) are the numbers of all,
 *            odd-length and even-length cycles of g on F_2^n. The sum runs
 *            over conjugacy classes (rational canonical forms:...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a832_addsh, a832_divs, a832_g, a832_muls, a832_pdeg, a832_pmod,
 *            a832_shr, a832_tot_fn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a832_addsh.h"
#include "../utils/a832_divs.h"
#include "../utils/a832_g.h"
#include "../utils/a832_muls.h"
#include "../utils/a832_pdeg.h"
#include "../utils/a832_pmod.h"
#include "../utils/a832_shr.h"
#include "../utils/a832_tot_fn.h"

static int a832_n, a832_ni, a832_cap;
static uint64_t *a832_poly = 0, *a832_ord = 0;
static int *a832_deg = 0;
static int a832_nc, a832_np;
static int a832_cf[128], a832_cstart[128], a832_clen[128], a832_parts[256];
static uint32_t *a832_tot = 0, *a832_odd = 0, *a832_tmp = 0;
static size_t a832_L, a832_Lo;

static int a832_F(uint64_t d)
{
    int s = 0, F = 0, c, k;
    uint64_t dp = d, v;
    while (!(dp & 1)) { dp >>= 1; s++; }
    v = s >= 40 ? ((uint64_t)1 << 40) : ((uint64_t)1 << s);
    for (c = 0; c < a832_nc; c++) {
        int fi = a832_cf[c];
        if (dp % a832_ord[fi] != 0) continue;
        for (k = 0; k < a832_clen[c]; k++) {
            uint64_t p = (uint64_t)a832_parts[a832_cstart[c] + k];
            F += a832_deg[fi] * (int)(p < v ? p : v);
        }
    }
    return F;
}

static uint64_t a832_cyc(uint64_t M)
{
    unsigned __int128 s = 0;
    uint64_t d;
    for (d = 1; d * d <= M; d++)
        if (M % d == 0) {
            s += (unsigned __int128)a832_tot_fn(M / d) << a832_F(d);
            if (d * d != M) s += (unsigned __int128)a832_tot_fn(d) << a832_F(M / d);
        }
    return (uint64_t)(s / M);
}

static void a832_eval(void)
{
    int n = a832_n, c, k;
    uint64_t P = 1, Po, cy, o, e, sh0;
    long long E = 0;
    for (c = 0; c < a832_nc; c++) {
        int m = a832_parts[a832_cstart[c]];
        uint64_t p2 = 1, x;
        while (p2 < (uint64_t)m) p2 <<= 1;
        x = a832_ord[a832_cf[c]] * p2;
        P = P / a832_g(P, x) * x;
    }
    Po = P;
    while (!(Po & 1)) Po >>= 1;
    cy = a832_cyc(P);
    o = a832_cyc(Po);
    e = cy - o;

    memcpy(a832_tmp, a832_odd, a832_Lo * sizeof(uint32_t));
    for (c = 0; c < a832_nc; c++) {
        int d = a832_deg[a832_cf[c]], st = a832_cstart[c], len = a832_clen[c];
        int maxp = a832_parts[st], j;
        long long s2 = 0;
        for (j = 1; j <= maxp; j++) {
            long long cnt = 0;
            for (k = 0; k < len; k++) if (a832_parts[st + k] >= j) cnt++;
            s2 += cnt * cnt;
        }
        k = 0;
        while (k < len) {
            int v = a832_parts[st + k], m = 0;
            while (k < len && a832_parts[st + k] == v) { m++; k++; }
            s2 -= (long long)m * (m + 1) / 2;
            for (j = 1; j <= m; j++)
                a832_divs(a832_tmp, a832_Lo, (uint32_t)(((uint64_t)1 << (d * j)) - 1));
        }
        E += (long long)d * s2;
    }
    sh0 = (uint64_t)((long long)n * (n - 1) / 2 - E);
    a832_addsh(a832_tot, a832_L, a832_tmp, a832_Lo, sh0 + 2 * cy);
    a832_addsh(a832_tot, a832_L, a832_tmp, a832_Lo, sh0 + o + 2 * e);
}

static void a832_rec(int idx, int rem);

static void a832_part(int idx, int rem, int k, int maxp)
{
    int p;
    if (k == 0) {
        a832_clen[a832_nc - 1] = a832_np - a832_cstart[a832_nc - 1];
        a832_rec(idx + 1, rem);
        return;
    }
    for (p = (maxp < k ? maxp : k); p >= 1; p--) {
        a832_parts[a832_np++] = p;
        a832_part(idx, rem, k - p, p);
        a832_np--;
    }
}

static void a832_rec(int idx, int rem)
{
    int d, k;
    if (rem == 0) { a832_eval(); return; }
    if (idx >= a832_ni || a832_deg[idx] > rem) return;
    a832_rec(idx + 1, rem);
    d = a832_deg[idx];
    for (k = 1; k * d <= rem; k++) {
        a832_cf[a832_nc] = idx;
        a832_cstart[a832_nc] = a832_np;
        a832_nc++;
        a832_part(idx, rem - k * d, k, k);
        a832_nc--;
    }
}

static void a832_add_irr(uint64_t f, int d)
{
    uint64_t r = 1, k = 0;
    if (a832_ni == a832_cap) {
        a832_cap = a832_cap ? 2 * a832_cap : 64;
        a832_poly = (uint64_t *)realloc(a832_poly, (size_t)a832_cap * sizeof(uint64_t));
        a832_ord = (uint64_t *)realloc(a832_ord, (size_t)a832_cap * sizeof(uint64_t));
        a832_deg = (int *)realloc(a832_deg, (size_t)a832_cap * sizeof(int));
    }
    do {
        r <<= 1;
        if ((r >> d) & 1) r ^= f;
        k++;
    } while (r != 1);
    a832_poly[a832_ni] = f;
    a832_deg[a832_ni] = d;
    a832_ord[a832_ni] = k;
    a832_ni++;
}

seq_t a(idx_t n)
{
    int N = (int)n, d, i;
    uint64_t f;
    seq_t r = 0;
    size_t top;
    if (n < 1) return 0;
    a832_n = N;
    a832_ni = 0;
    for (d = 1; d <= N; d++) {
        for (f = ((uint64_t)1 << d) | 1; f < ((uint64_t)1 << (d + 1)); f += 2) {
            int ok = 1;
            for (i = 0; i < a832_ni && 2 * a832_deg[i] <= d; i++)
                if (a832_pmod(f, a832_poly[i]) == 0) { ok = 0; break; }
            if (ok) a832_add_irr(f, d);
        }
    }
    a832_L = (size_t)((((uint64_t)1 << (N + 1)) + (uint64_t)N * N + 64) / 32 + 4);
    a832_Lo = (size_t)((uint64_t)N * (N + 1) / 2 / 32 + 2);
    a832_tot = (uint32_t *)calloc(a832_L, sizeof(uint32_t));
    a832_odd = (uint32_t *)calloc(a832_Lo, sizeof(uint32_t));
    a832_tmp = (uint32_t *)calloc(a832_Lo, sizeof(uint32_t));
    a832_odd[0] = 1;
    for (i = 1; i <= N; i++)
        a832_muls(a832_odd, a832_Lo, (uint32_t)(((uint64_t)1 << i) - 1));
    a832_nc = 0;
    a832_np = 0;
    a832_rec(0, N);

    a832_shr(a832_tot, a832_L, (uint64_t)N * (N - 1) / 2 + 1);
    for (i = 1; i <= N; i++)
        a832_divs(a832_tot, a832_L, (uint32_t)(((uint64_t)1 << i) - 1));

    top = a832_L;
    while (top > 0 && a832_tot[top - 1] == 0) top--;
    while (top-- > 0) {
        uint32_t w = a832_tot[top];
        int b;
        for (b = 24; b >= 0; b -= 8)
            r = r * 256 + (seq_t)((w >> b) & 255);
    }
    free(a832_tot);
    free(a832_odd);
    free(a832_tmp);
    a832_tot = a832_odd = a832_tmp = 0;
    return r;
}
