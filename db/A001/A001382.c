/*
 * A001382  Weight distribution of ternary [ 24,12,9 ] quadratic residue code
 *          (also of Pless symmetry code).
 * https://oeis.org/A001382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 3n in the extended ternary
 *            [24,12,9] quadratic residue code; the generator polynomial g(x)
 *            = gcd(e(x), x^23-1) is found from a QR idempotent e = a +
 *            b*sum_{r QR} x^r + c*sum_{r NR} x^r over GF(3), all 3^12
 *            codewords are enumerated, and the weights are counted
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 9 terms through n = 8 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static int a1382_deg(const int *p)
{
    int i;
    for (i = 23; i >= 0; i--)
        if (p[i]) return i;
    return -1;
}

/* a <- a mod b over GF(3) */
static void a1382_mod(int *a, const int *b)
{
    int db = a1382_deg(b), da = a1382_deg(a), i;
    int inv = b[db]; /* inverse of 1 is 1, of 2 is 2 mod 3 */
    while (da >= db) {
        int c = (a[da] * inv) % 3;
        for (i = 0; i <= db; i++)
            a[i + da - db] = ((a[i + da - db] - c * b[i]) % 3 + 3) % 3;
        da = a1382_deg(a);
    }
}

static void a1382_gcd(int *a, int *b, int *out)
{
    int x[24], y[24], t[24];
    memcpy(x, a, sizeof x);
    memcpy(y, b, sizeof y);
    while (a1382_deg(y) >= 0) {
        memcpy(t, x, sizeof t);
        a1382_mod(t, y);
        memcpy(x, y, sizeof x);
        memcpy(y, t, sizeof y);
    }
    memcpy(out, x, sizeof x);
}

static void a1382_rec(int k, int G[12][24], int cur[13][24], uint64_t *cnt)
{
    int c, j;
    if (k == 12) {
        int w = 0;
        for (j = 0; j < 24; j++)
            if (cur[12][j]) w++;
        cnt[w]++;
        return;
    }
    for (c = 0; c < 3; c++) {
        for (j = 0; j < 24; j++)
            cur[k + 1][j] = (cur[k][j] + c * G[k][j]) % 3;
        a1382_rec(k + 1, G, cur, cnt);
    }
}

static uint64_t a1382_cnt[25];
static int a1382_done = 0;

static void a1382_init(void)
{
    int isqr[23];
    int i, j, a0, b0, c0;
    int g[24];
    int found = 0;
    memset(isqr, 0, sizeof isqr);
    for (i = 1; i < 23; i++)
        isqr[(i * i) % 23] = 1;
    for (a0 = 0; a0 < 3 && !found; a0++)
        for (b0 = 0; b0 < 3 && !found; b0++)
            for (c0 = 0; c0 < 3 && !found; c0++) {
                int e[24], m[24];
                memset(e, 0, sizeof e);
                memset(m, 0, sizeof m);
                e[0] = a0;
                for (i = 1; i < 23; i++)
                    e[i] = isqr[i] ? b0 : c0;
                if (a1382_deg(e) < 0) continue;
                m[23] = 1;
                m[0] = 2; /* x^23 - 1 */
                a1382_gcd(m, e, g);
                if (a1382_deg(g) == 11) found = 1;
            }
    {
        int G[12][24];
        int cur[13][24];
        memset(G, 0, sizeof G);
        for (i = 0; i < 12; i++) {
            int s = 0;
            for (j = 0; j <= 11; j++)
                G[i][i + j] = g[j];
            for (j = 0; j < 23; j++)
                s += G[i][j];
            G[i][23] = (3 - s % 3) % 3;
        }
        memset(cur, 0, sizeof cur);
        memset(a1382_cnt, 0, sizeof a1382_cnt);
        a1382_rec(0, G, cur, a1382_cnt);
    }
    a1382_done = 1;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n > 8) return r;
    if (!a1382_done) a1382_init();
    r += (idx_t)a1382_cnt[3 * n];
    return r;
}
