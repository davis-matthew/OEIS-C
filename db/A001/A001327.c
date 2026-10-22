/*
 * A001327  Number of equivalence classes of 3-valued Post functions of n
 *          variables under action of semi-direct product of symmetric groups
 *          S_n and S(n,3).
 * https://oeis.org/A001327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma on the wreath product S_3 wr S_n acting on the
 *            3^n points of {0,1,2}^n: a(n) = (1/(6^n n!)) * Sum_{g in S_3 wr
 *            S_n} 3^{c(g)}, where c(g) is the number of cycles of g. A
 *            k-cycle of the variable permutation with twist h in S_3 acts on
 *            {0,1,2}^k with fix(T^j) = fix(h^{j/d})^d, d = g...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a001327_fixh, a001327_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001327_fixh.h"
#include "../utils/a001327_gcd.h"

typedef struct { uint64_t len, cnt; } a001327_cyc;
typedef unsigned __int128 a001327_u128;

#define A001327_CAP 8192

static int a001327_n;
static a001327_u128 a001327_factn;
static a001327_u128 *a001327_coef;
static a001327_cyc *a001327_ft[64][3];
static int a001327_fn[64][3];

static int a001327_ftype(uint64_t k, int cls, a001327_cyc *O)
{
    uint64_t ord = cls == 0 ? 1 : (cls == 1 ? 2 : 3);
    uint64_t L = k * ord, m, e, j;
    uint64_t *P = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    int no = 0;
    for (m = 1; m <= L; m++) {
        uint64_t d, f, fm, sub;
        if (L % m) continue;
        d = a001327_gcd(m, k);
        f = a001327_fixh(cls, m / d);
        fm = 1;
        for (j = 0; j < d; j++) fm *= f;
        sub = 0;
        for (e = 1; e < m; e++) if (m % e == 0) sub += P[e];
        P[m] = fm - sub;
        if (P[m]) { O[no].len = m; O[no].cnt = P[m] / m; no++; }
    }
    free(P);
    return no;
}

static int a001327_comb(const a001327_cyc *A, int na, const a001327_cyc *B, int nb, a001327_cyc *O)
{
    int no = 0, i, j, q;
    for (i = 0; i < na; i++)
        for (j = 0; j < nb; j++) {
            uint64_t g = a001327_gcd(A[i].len, B[j].len);
            uint64_t l = A[i].len / g * B[j].len;
            uint64_t c = A[i].cnt * B[j].cnt * g;
            for (q = 0; q < no; q++) if (O[q].len == l) break;
            if (q < no) O[q].cnt += c;
            else if (no < A001327_CAP) { O[no].len = l; O[no].cnt = c; no++; }
        }
    return no;
}

static void a001327_rec(int k, int rem, const a001327_cyc *ty, int nt,
                        a001327_u128 num, a001327_u128 den)
{
    int m, a, b, c, i;
    a001327_cyc *buf1, *buf2;
    if (rem == 0) {
        uint64_t t = 0;
        for (i = 0; i < nt; i++) t += ty[i].cnt;
        a001327_coef[t] += (a001327_factn / den) * num;
        return;
    }
    if (k == 0) return;
    buf1 = (a001327_cyc *)malloc(sizeof(a001327_cyc) * A001327_CAP);
    buf2 = (a001327_cyc *)malloc(sizeof(a001327_cyc) * A001327_CAP);
    for (m = 0; m * k <= rem; m++) {
        for (a = 0; a <= m; a++)
            for (b = 0; a + b <= m; b++) {
                a001327_cyc *cur = buf1, *oth = buf2, *sw;
                int ncur = nt, r;
                a001327_u128 num2 = num, den2 = den;
                c = m - a - b;
                memcpy(cur, ty, sizeof(a001327_cyc) * (size_t)nt);
                for (r = 0; r < a + b + c; r++) {
                    int cls = r < a ? 0 : (r < a + b ? 1 : 2);
                    ncur = a001327_comb(cur, ncur, a001327_ft[k][cls], a001327_fn[k][cls], oth);
                    sw = cur; cur = oth; oth = sw;
                }
                for (r = 0; r < (k - 1) * m; r++) num2 *= 6;
                for (r = 0; r < b; r++) num2 *= 3;
                for (r = 0; r < c; r++) num2 *= 2;
                for (r = 0; r < m; r++) den2 *= (a001327_u128)k;
                for (r = 2; r <= a; r++) den2 *= (a001327_u128)r;
                for (r = 2; r <= b; r++) den2 *= (a001327_u128)r;
                for (r = 2; r <= c; r++) den2 *= (a001327_u128)r;
                a001327_rec(k - 1, rem - k * m, cur, ncur, num2, den2);
            }
    }
    free(buf1);
    free(buf2);
}

seq_t a(idx_t n)
{
    int nn = (int)n, k, cls, i;
    uint64_t T = 1, t;
    size_t limbs, len, j;
    uint32_t *N;
    a001327_cyc init;
    seq_t r = 0;

    a001327_n = nn;
    for (i = 0; i < nn; i++) T *= 3;
    a001327_factn = 1;
    for (i = 2; i <= nn; i++) a001327_factn *= (a001327_u128)i;
    a001327_coef = (a001327_u128 *)calloc(T + 1, sizeof(a001327_u128));
    for (k = 1; k <= nn; k++)
        for (cls = 0; cls < 3; cls++) {
            a001327_ft[k][cls] = (a001327_cyc *)malloc(sizeof(a001327_cyc) * (3 * (size_t)k + 1));
            a001327_fn[k][cls] = a001327_ftype((uint64_t)k, cls, a001327_ft[k][cls]);
        }
    init.len = 1; init.cnt = 1;
    a001327_rec(nn, nn, &init, 1, 1, 1);

    limbs = (size_t)(T / 20 + 16);
    N = (uint32_t *)calloc(limbs, sizeof(uint32_t));
    len = 1;
    for (t = T + 1; t-- > 0;) {
        a001327_u128 carry = a001327_coef[t];
        for (j = 0; j < len; j++) {
            a001327_u128 cur = (a001327_u128)N[j] * 3 + carry;
            N[j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        while (carry) { N[len++] = (uint32_t)carry; carry >>= 32; }
    }
    for (i = 0; i < nn; i++) {
        uint64_t rm = 0;
        for (j = len; j-- > 0;) { rm = (rm << 32) | N[j]; N[j] = (uint32_t)(rm / 6); rm %= 6; }
    }
    for (i = 2; i <= nn; i++) {
        uint64_t rm = 0;
        for (j = len; j-- > 0;) { rm = (rm << 32) | N[j]; N[j] = (uint32_t)(rm / (uint64_t)i); rm %= (uint64_t)i; }
    }
    while (len > 1 && N[len - 1] == 0) len--;
    for (j = len; j-- > 0;) {
        r *= 65536;
        r += (N[j] >> 16);
        r *= 65536;
        r += (N[j] & 0xffffu);
    }

    free(N);
    free(a001327_coef);
    for (k = 1; k <= nn; k++)
        for (cls = 0; cls < 3; cls++) free(a001327_ft[k][cls]);
    return r;
}
