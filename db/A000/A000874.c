/*
 * A000874  Number of switching networks with S(n) and C(2,2) acting on the
 *          domain and GL(2,2) acting on the range.
 * https://oeis.org/A000874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(6*2^n*n!)) * Sum_{g in B_n} (4^c(g) + 3*2^(c(g)+e(g))
 *            + 2*4^t(g)), where B_n = S(n) x C(2,2)^n (permutations and
 *            complementations of the n inputs) acts on the 2^n domain points,
 *            c(g) = number of cycles of g, e(g) = number of cycles of even
 *            length, t(g) = number of cycles of length divi...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a874_div, a874_gcd, a874_mu, a874_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a874_div.h"
#include "../utils/a874_gcd.h"
#include "../utils/a874_mu.h"
#include "../utils/a874_mul.h"

static int a874_n;
static int a874_pa[128], a874_pb[128];
static uint32_t *a874_S;
static size_t a874_W;
static uint32_t *a874_C;
static size_t a874_LC;

/* S += coef * C * 2^sh */
static void a874_addsh(uint32_t coef, uint64_t sh)
{
    size_t off = (size_t)(sh >> 5);
    unsigned b = (unsigned)(sh & 31);
    uint64_t carry = 0, acc = 0;
    uint32_t prev = 0;
    size_t i, j;
    for (i = 0; i < a874_LC + 2; i++) {
        uint64_t p = (i < a874_LC ? (uint64_t)a874_C[i] * coef : 0) + carry;
        uint32_t pw = (uint32_t)p;
        uint32_t sw;
        carry = p >> 32;
        if (b) sw = (uint32_t)(((uint64_t)pw << b) | ((uint64_t)prev >> (32 - b)));
        else sw = pw;
        prev = pw;
        acc += (uint64_t)a874_S[off + i] + sw;
        a874_S[off + i] = (uint32_t)acc;
        acc >>= 32;
    }
    for (j = off + a874_LC + 2; acc && j < a874_W; j++) {
        acc += a874_S[j];
        a874_S[j] = (uint32_t)acc;
        acc >>= 32;
    }
}

static void a874_proc(void)
{
    int n = a874_n, k, t;
    long long L = 1, d, c = 0, ce = 0, c3 = 0;
    long long *dv, *F;
    size_t nd = 0, i, j;

    for (k = 1; k <= n; k++) {
        if (a874_pa[k]) L = L / a874_gcd(L, k) * k;
        if (a874_pb[k]) L = L / a874_gcd(L, 2 * k) * (2 * k);
    }
    for (d = 1; d <= L; d++) if (L % d == 0) nd++;
    dv = (long long *)malloc(nd * sizeof(long long));
    F = (long long *)malloc(nd * sizeof(long long));
    nd = 0;
    for (d = 1; d <= L; d++) if (L % d == 0) dv[nd++] = d;

    for (i = 0; i < nd; i++) {
        int zero = 0;
        long long e = 0;
        d = dv[i];
        for (k = 1; k <= n; k++) {
            long long g = a874_gcd(d, k);
            if (a874_pa[k]) e += g * a874_pa[k];
            if (a874_pb[k]) {
                if ((d / g) & 1) zero = 1;
                else e += g * a874_pb[k];
            }
        }
        F[i] = zero ? 0 : (1LL << e);
    }
    for (i = 0; i < nd; i++) {
        long long s = 0;
        d = dv[i];
        for (j = 0; j <= i; j++)
            if (d % dv[j] == 0) s += a874_mu(d / dv[j]) * F[j];
        s /= d;
        c += s;
        if (d % 2 == 0) ce += s;
        if (d % 3 == 0) c3 += s;
    }
    free(dv);
    free(F);

    /* class size = 2^n n! / prod_k (2k)^(a_k+b_k) a_k! b_k! */
    memset(a874_C, 0, a874_LC * sizeof(uint32_t));
    a874_C[0] = 1;
    for (k = 1; k <= n; k++) a874_mul(a874_C, a874_LC, 2u * (uint32_t)k);
    for (k = 1; k <= n; k++) {
        for (t = 0; t < a874_pa[k] + a874_pb[k]; t++)
            a874_div(a874_C, a874_LC, 2u * (uint32_t)k);
        for (t = 2; t <= a874_pa[k]; t++) a874_div(a874_C, a874_LC, (uint32_t)t);
        for (t = 2; t <= a874_pb[k]; t++) a874_div(a874_C, a874_LC, (uint32_t)t);
    }

    a874_addsh(1, (uint64_t)(2 * c));
    a874_addsh(3, (uint64_t)(c + ce));
    a874_addsh(2, (uint64_t)(2 * c3));
}

static void a874_rec(int k, int rem)
{
    int x, y;
    if (k == 0) {
        if (rem == 0) a874_proc();
        return;
    }
    for (x = 0; x * k <= rem; x++) {
        for (y = 0; (x + y) * k <= rem; y++) {
            a874_pa[k] = x;
            a874_pb[k] = y;
            a874_rec(k - 1, rem - (x + y) * k);
        }
    }
    a874_pa[k] = 0;
    a874_pb[k] = 0;
}

seq_t a(idx_t n)
{
    int nn = (int)n, i, sh;
    size_t top;
    seq_t r = 0;

    a874_n = nn;
    memset(a874_pa, 0, sizeof(a874_pa));
    memset(a874_pb, 0, sizeof(a874_pb));
    a874_LC = (size_t)(8 * nn + 64) / 32 + 2;
    a874_W = (((size_t)1 << (nn + 1)) >> 5) + a874_LC + 6;
    a874_S = (uint32_t *)calloc(a874_W, sizeof(uint32_t));
    a874_C = (uint32_t *)calloc(a874_LC, sizeof(uint32_t));

    a874_rec(nn, nn);

    a874_div(a874_S, a874_W, 6);
    for (i = 1; i <= nn; i++) a874_div(a874_S, a874_W, 2u * (uint32_t)i);

    top = a874_W;
    while (top > 0 && a874_S[top - 1] == 0) top--;
    while (top-- > 0) {
        for (sh = 28; sh >= 0; sh -= 4) {
            r = (seq_t)(r * 16);
            r = (seq_t)(r + ((a874_S[top] >> sh) & 15u));
        }
    }
    free(a874_S);
    free(a874_C);
    return r;
}
