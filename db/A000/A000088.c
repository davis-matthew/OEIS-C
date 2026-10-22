/*
 * A000088  Number of simple graphs on n unlabeled nodes.
 * https://oeis.org/A000088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, nice, changed
 * Formula:   a(n) = (1/n!) * Sum_{c : 1*c_1+2*c_2+...+n*c_n=n} (n!/z(c)) *
 *            2^f(c), z(c) = Product_j j^c_j*c_j!, f(c) = Sum_j
 *            (c_j*floor(j/2) + j*binomial(c_j,2)) + Sum_{i<j}
 *            c_i*c_j*gcd(i,j) (Polya cycle index of the pair group). Cycle
 *            types with parts >= 3 are found by a search; the 2-cycles and
 *            fixed points...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long a88_u64;
typedef unsigned __int128 a88_u128;

static a88_u64 a88_N;
static int a88_L;
static size_t a88_W;
static a88_u64 *a88_buf;
static a88_u64 *a88_P;
static size_t *a88_off;
static a88_u64 a88_pp[1024];
static a88_u64 a88_pc[1024];

static a88_u64 a88_gcd(a88_u64 x, a88_u64 y)
{
    while (y) { a88_u64 t = x % y; x = y; y = t; }
    return x;
}

static void a88_mul_small(a88_u64 *x, size_t len, a88_u64 m)
{
    a88_u128 c = 0;
    size_t j;
    if (m == 1) return;
    for (j = 0; j < len; j++) {
        c += (a88_u128)x[j] * m;
        x[j] = (a88_u64)c;
        c >>= 64;
    }
}

/* exact division of x by d (d divides x) */
static void a88_divexact(a88_u64 *x, size_t len, a88_u64 d)
{
    int s = __builtin_ctzll(d);
    size_t j;
    a88_u64 o = d >> s;
    if (s) {
        for (j = 0; j + 1 < len; j++)
            x[j] = (x[j] >> s) | (x[j + 1] << (64 - s));
        x[len - 1] >>= s;
    }
    if (o > 1) {
        a88_u64 inv = o, borrow = 0;
        int it;
        for (it = 0; it < 6; it++) inv *= 2 - o * inv;
        for (j = 0; j < len; j++) {
            a88_u64 xj = x[j];
            a88_u64 b1 = xj < borrow;
            a88_u64 t = xj - borrow;
            a88_u64 q = t * inv;
            x[j] = q;
            borrow = (a88_u64)(((a88_u128)q * o) >> 64) + b1;
        }
    }
}

/* dst += src << sh */
static void a88_add_shifted(a88_u64 *dst, size_t dlen, const a88_u64 *src,
                            size_t slen, a88_u64 sh)
{
    size_t w = (size_t)(sh / 64), j, id;
    int s = (int)(sh % 64);
    a88_u64 prev = 0;
    a88_u128 c = 0;
    for (j = 0; j <= slen && w + j < dlen; j++) {
        a88_u64 cur = j < slen ? src[j] : 0;
        a88_u64 v = s ? ((cur << s) | (prev >> (64 - s))) : cur;
        prev = cur;
        c += (a88_u128)dst[w + j] + v;
        dst[w + j] = (a88_u64)c;
        c >>= 64;
    }
    id = w + j;
    while (c && id < dlen) {
        c += dst[id];
        dst[id] = (a88_u64)c;
        c >>= 64;
        id++;
    }
}

static size_t a88_idx(a88_u64 m, a88_u64 o, a88_u64 e)
{
    return a88_off[m] + (size_t)(o * (m / 4 + 1) + e);
}

/* parts >= 3 only; o = number of odd parts, ev = number of even parts */
static void a88_dfs(a88_u64 rem, a88_u64 maxp, a88_u64 f, int depth,
                    a88_u64 o, a88_u64 ev)
{
    a88_u64 *cls = a88_buf + (size_t)depth * a88_L;
    a88_u64 *nx = cls + a88_L;
    a88_u64 m = a88_N - rem;
    a88_u64 i, top;
    a88_add_shifted(a88_P + a88_idx(m, o, ev) * a88_W, a88_W, cls,
                    (size_t)a88_L, f);
    top = rem < maxp ? rem : maxp;
    for (i = top; i >= 3; i--) {
        a88_u64 cross = 0, ff = f, r = rem, k;
        int d;
        for (d = 0; d < depth; d++)
            cross += a88_pc[d] * a88_gcd(i, a88_pp[d]);
        memcpy(nx, cls, sizeof(a88_u64) * (size_t)a88_L);
        for (k = 1; k * i <= rem; k++) {
            a88_u64 acc = 1, t;
            for (t = 0; t < i; t++) {
                a88_u64 v = r - t;
                if (acc > (~0ULL) / v) { a88_mul_small(nx, (size_t)a88_L, acc); acc = 1; }
                acc *= v;
            }
            a88_mul_small(nx, (size_t)a88_L, acc);
            a88_divexact(nx, (size_t)a88_L, i * k);
            r -= i;
            ff += i / 2 + i * (k - 1) + cross;
            a88_pp[depth] = i;
            a88_pc[depth] = k;
            if (i & 1) a88_dfs(r, i - 1, ff, depth + 1, o + k, ev);
            else a88_dfs(r, i - 1, ff, depth + 1, o, ev + k);
        }
    }
}

seq_t a(idx_t n)
{
    a88_u64 N = (a88_u64)n, bits = 1, k, m, o, e;
    size_t E, TW, j;
    a88_u64 *T, *tmp, *H;
    seq_t r = 0;
    long long top;

    a88_N = N;
    for (k = 2; k <= N; k++) bits += 64 - __builtin_clzll(k);
    a88_L = (int)(bits / 64 + 2);
    a88_W = (size_t)((N * (N - 1) / 2 + bits) / 64 + 3);

    a88_off = (size_t *)calloc((size_t)N + 2, sizeof(size_t));
    E = 0;
    for (m = 0; m <= N; m++) {
        a88_off[m] = E;
        E += (size_t)((m / 3 + 1) * (m / 4 + 1));
    }
    a88_P = (a88_u64 *)calloc(E * a88_W, sizeof(a88_u64));
    a88_buf = (a88_u64 *)calloc((size_t)(N + 3) * a88_L, sizeof(a88_u64));
    a88_buf[0] = 1;
    a88_dfs(N, N, 0, 0, 0, 0);

    TW = a88_W + (size_t)a88_L + 2;
    T = (a88_u64 *)calloc(TW, sizeof(a88_u64));
    tmp = (a88_u64 *)calloc(a88_W + (size_t)a88_L + 1, sizeof(a88_u64));
    H = (a88_u64 *)calloc((size_t)a88_L, sizeof(a88_u64));

    for (m = 0; m <= N; m++) {
        a88_u64 s = N - m;
        for (o = 0; o <= m / 3; o++) {
            for (e = 0; e <= m / 4; e++) {
                const a88_u64 *Pe = a88_P + a88_idx(m, o, e) * a88_W;
                size_t pl = a88_W, tl;
                a88_u64 c2;
                while (pl > 0 && Pe[pl - 1] == 0) pl--;
                if (pl == 0) continue;
                memset(H, 0, sizeof(a88_u64) * (size_t)a88_L);
                H[0] = 1;
                for (c2 = 0; 2 * c2 <= s; c2++) {
                    a88_u64 c1, ex;
                    size_t hj;
                    if (c2 > 0) {
                        a88_u64 q = s - 2 * (c2 - 1);
                        a88_mul_small(H, (size_t)a88_L, q * (q - 1));
                        a88_divexact(H, (size_t)a88_L, 2 * c2);
                    }
                    c1 = s - 2 * c2;
                    ex = c2 + c2 * (c2 - 1) + c2 * (o + 2 * e)
                       + c1 * (c1 - 1) / 2 + c1 * (o + e + c2);
                    tl = pl + (size_t)a88_L;
                    memset(tmp, 0, sizeof(a88_u64) * (tl + 1));
                    for (hj = 0; hj < (size_t)a88_L; hj++) {
                        a88_u128 c = 0;
                        size_t pj;
                        a88_u64 hv = H[hj];
                        if (!hv) continue;
                        for (pj = 0; pj < pl; pj++) {
                            c += (a88_u128)Pe[pj] * hv + tmp[hj + pj];
                            tmp[hj + pj] = (a88_u64)c;
                            c >>= 64;
                        }
                        for (pj = hj + pl; c && pj <= tl; pj++) {
                            c += tmp[pj];
                            tmp[pj] = (a88_u64)c;
                            c >>= 64;
                        }
                    }
                    a88_add_shifted(T, TW, tmp, tl, ex);
                }
            }
        }
    }

    for (k = 2; k <= N; k++) a88_divexact(T, TW, k);

    top = (long long)TW - 1;
    while (top > 0 && T[top] == 0) top--;
    for (; top >= 0; top--) {
        int q;
        for (q = 0; q < 8; q++) r = r * 256u;
        r += (seq_t)T[top];
    }
    (void)j;

    free(H);
    free(tmp);
    free(T);
    free(a88_buf);
    free(a88_P);
    free(a88_off);
    return r;
}
