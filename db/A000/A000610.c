/*
 * A000610  Number of self-complementary Boolean functions of n variables: see
 *          Comments for precise definition.
 * https://oeis.org/A000610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/(2^n*n!)) * Sum_{g in B_n} [every cycle of g on
 *            {0,1}^n has even length] * 2^(number of cycles of g on {0,1}^n),
 *            i.e. Burnside count of the classes of Boolean functions under
 *            the hyperoctahedral group B_n that are fixed by complementation;
 *            the sum runs over signed cycle types of B_n (cl...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   a610_div, a610_gcd, a610_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a610_div.h"
#include "../utils/a610_gcd.h"
#include "../utils/a610_mul.h"

typedef struct { uint64_t len, cnt; } a610_cyc;

static a610_cyc *a610_base[64][2];
static int a610_bn[64][2];
static int a610_mult[64][2];
static int a610_n;
static size_t a610_L, a610_Ls;
static uint32_t *a610_tot, *a610_tmp;

/* cycle-length distribution of a signed k-cycle (s = 1: negative) acting on {0,1}^k */
static void a610_make_base(int k, int s)
{
    uint64_t N = (uint64_t)1 << k, mask = N - 1, x, y, len;
    unsigned char *vis = (unsigned char *)calloc((size_t)N, 1);
    uint64_t *cnt = (uint64_t *)calloc((size_t)(2 * k + 1), sizeof(uint64_t));
    int m = 0, i;
    for (x = 0; x < N; x++) {
        if (vis[x]) continue;
        len = 0; y = x;
        do {
            vis[y] = 1;
            y = ((y << 1) | (y >> (k - 1))) & mask;
            if (s) y ^= 1;
            len++;
        } while (y != x);
        cnt[len]++;
    }
    a610_base[k][s] = (a610_cyc *)malloc((size_t)(2 * k + 1) * sizeof(a610_cyc));
    for (i = 1; i <= 2 * k; i++)
        if (cnt[i]) { a610_base[k][s][m].len = (uint64_t)i; a610_base[k][s][m].cnt = cnt[i]; m++; }
    a610_bn[k][s] = m;
    free(vis); free(cnt);
}

static int a610_combine(const a610_cyc *D, int dn, const a610_cyc *B, int bn, a610_cyc *out)
{
    int m = 0, i, j, t;
    for (i = 0; i < dn; i++)
        for (j = 0; j < bn; j++) {
            uint64_t g = a610_gcd(D[i].len, B[j].len);
            uint64_t l = D[i].len / g * B[j].len, c = D[i].cnt * B[j].cnt * g;
            for (t = 0; t < m && out[t].len != l; t++) ;
            if (t == m) { out[m].len = l; out[m].cnt = 0; m++; }
            out[t].cnt += c;
        }
    return m;
}

static void a610_addsh(uint64_t c)
{
    size_t off = (size_t)(c / 32), i, j = off;
    unsigned sh = (unsigned)(c % 32);
    uint64_t prev = 0, carry = 0;
    for (i = 0; i <= a610_Ls && j < a610_L; i++, j++) {
        uint64_t cur = (i < a610_Ls) ? a610_tmp[i] : 0;
        uint32_t word = (uint32_t)(((cur << 32) | prev) >> (32 - sh));
        uint64_t acc = (uint64_t)a610_tot[j] + word + carry;
        a610_tot[j] = (uint32_t)acc; carry = acc >> 32;
        prev = cur;
    }
    while (carry && j < a610_L) {
        uint64_t acc = (uint64_t)a610_tot[j] + carry;
        a610_tot[j] = (uint32_t)acc; carry = acc >> 32; j++;
    }
}

static void a610_leaf(const a610_cyc *D, int dn)
{
    uint64_t c = 0; int i, k, s, j;
    for (i = 0; i < dn; i++) { if (D[i].len & 1) return; c += D[i].cnt; }
    memset(a610_tmp, 0, a610_Ls * sizeof(uint32_t));
    a610_tmp[0] = 1;
    for (i = 1; i <= a610_n; i++) a610_mul(a610_tmp, a610_Ls, (uint32_t)(2 * i));
    for (k = 1; k <= a610_n; k++)
        for (s = 0; s < 2; s++)
            for (j = 1; j <= a610_mult[k][s]; j++) {
                a610_div(a610_tmp, a610_Ls, (uint32_t)(2 * k));
                a610_div(a610_tmp, a610_Ls, (uint32_t)j);
            }
    a610_addsh(c);
}

static void a610_rec(int r, int k, int s, const a610_cyc *D, int dn)
{
    int kk, ss;
    a610_cyc *E;
    if (r == 0) { a610_leaf(D, dn); return; }
    E = (a610_cyc *)malloc((size_t)dn * (size_t)(2 * a610_n + 1) * sizeof(a610_cyc));
    for (kk = (k < r ? k : r); kk >= 1; kk--)
        for (ss = (kk == k ? s : 1); ss >= 0; ss--) {
            int en = a610_combine(D, dn, a610_base[kk][ss], a610_bn[kk][ss], E);
            a610_mult[kk][ss]++;
            a610_rec(r - kk, kk, ss, E, en);
            a610_mult[kk][ss]--;
        }
    free(E);
}

seq_t a(idx_t n)
{
    int N, k, s, i;
    size_t sb, j;
    seq_t res = 0;
    a610_cyc d0;
    if (n == 0 || n > 30) return 0;
    N = (int)n;
    a610_n = N;
    for (k = 1; k <= N; k++)
        for (s = 0; s < 2; s++)
            if (!a610_base[k][s]) a610_make_base(k, s);
    sb = (size_t)N;
    for (i = 1; i <= N; i++) { int t = i; while (t) { sb++; t >>= 1; } }
    a610_Ls = sb / 32 + 2;
    a610_L = ((size_t)1 << (N - 1)) / 32 + a610_Ls + 2;
    a610_tot = (uint32_t *)calloc(a610_L, sizeof(uint32_t));
    a610_tmp = (uint32_t *)calloc(a610_Ls, sizeof(uint32_t));
    memset(a610_mult, 0, sizeof(a610_mult));
    d0.len = 1; d0.cnt = 1;
    a610_rec(N, N, 1, &d0, 1);
    for (i = 1; i <= N; i++) a610_div(a610_tot, a610_L, (uint32_t)(2 * i));
    for (j = a610_L; j-- > 0;) {
        res <<= 16;
        res <<= 16;
        res += (seq_t)a610_tot[j];
    }
    free(a610_tot); free(a610_tmp);
    a610_tot = NULL; a610_tmp = NULL;
    return res;
}
