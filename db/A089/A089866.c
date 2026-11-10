/*
 * A089866  Permutation of natural numbers induced by Catalan Automorphism
 *          *A089866 acting on the parenthesizations/binary trees encoded by
 *          A014486/A063171.
 * https://oeis.org/A089866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486-rank of the tree (f(L), R), where the n-th Dyck
 *            word is (L)R and f is A074680 (f(nil)=nil; f((nil,B))=(B,nil);
 *            f(((A,B),C))=(A,(B,C))), computed by unranking, rewriting the
 *            word and re-ranking
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A089866_MAXK 30

static uint64_t A089866_W[2 * A089866_MAXK + 2][2 * A089866_MAXK + 3];
static bool A089866_ready = false;

static void A089866_init(void)
{
    int r, h;
    int R = 2 * A089866_MAXK + 1;
    int H = 2 * A089866_MAXK + 2;
    for (r = 0; r <= R; ++r)
        for (h = 0; h <= H; ++h)
            A089866_W[r][h] = 0;
    A089866_W[0][0] = 1;
    for (r = 1; r <= R; ++r) {
        for (h = 0; h < H; ++h) {
            uint64_t v = A089866_W[r - 1][h + 1];
            if (h > 0) v += A089866_W[r - 1][h - 1];
            A089866_W[r][h] = v;
        }
    }
    A089866_ready = true;
}

/* index of the ')' matching the '(' at s[0] */
static int A089866_match(const char *s)
{
    int d = 0, i = 0;
    for (;; ++i) {
        if (s[i] == '(') d++;
        else { d--; if (d == 0) return i; }
    }
}

static void A089866_put(char *out, int *pos, const char *src, int len)
{
    int i;
    for (i = 0; i < len; ++i) out[(*pos)++] = src[i];
}

seq_t a(idx_t n)
{
    char w[2 * A089866_MAXK + 4];
    char o[2 * A089866_MAXK + 4];
    int k = 0, len, pos, bal, rem, j, p;
    uint64_t m, rank;

    if (n == 0) return (seq_t)0;
    if (!A089866_ready) A089866_init();

    m = (uint64_t)n;
    while (k <= A089866_MAXK) {
        uint64_t c = A089866_W[2 * k][0];
        if (m < c) break;
        m -= c;
        ++k;
    }
    if (k > A089866_MAXK || k == 0) return (seq_t)0;
    len = 2 * k;

    /* unrank */
    bal = 0;
    for (pos = 0; pos < len; ++pos) {
        uint64_t cnt0 = 0;
        rem = len - pos;
        if (bal > 0) cnt0 = A089866_W[rem - 1][bal - 1];
        if (bal > 0 && m < cnt0) { w[pos] = ')'; bal--; }
        else { m -= cnt0; w[pos] = '('; bal++; }
    }

    /* transform */
    {
        int op = 0;
        const char *L, *R;
        int Ll, Rl;
        j = A089866_match(w);
        L = w + 1; Ll = j - 1;
        R = w + j + 1; Rl = len - j - 1;
        o[op++] = '(';
        if (Ll > 0) {
            int Xj = A089866_match(L);
            const char *Y = L + 1;
            int Yl = Xj - 1;
            const char *Z = L + Xj + 1;
            int Zl = Ll - Xj - 1;
            if (Yl == 0) {
                o[op++] = '(';
                A089866_put(o, &op, Z, Zl);
                o[op++] = ')';
            } else {
                const char *A, *B;
                int Al, Bl;
                p = A089866_match(Y);
                A = Y + 1; Al = p - 1;
                B = Y + p + 1; Bl = Yl - p - 1;
                o[op++] = '(';
                A089866_put(o, &op, A, Al);
                o[op++] = ')';
                o[op++] = '(';
                A089866_put(o, &op, B, Bl);
                o[op++] = ')';
                A089866_put(o, &op, Z, Zl);
            }
        }
        o[op++] = ')';
        A089866_put(o, &op, R, Rl);
    }

    /* rank */
    rank = 0;
    for (k = 0, pos = 0; pos < len; ++pos) { (void)k; break; }
    {
        int kk;
        for (kk = 0; kk < len / 2; ++kk) rank += A089866_W[2 * kk][0];
    }
    bal = 0;
    for (pos = 0; pos < len; ++pos) {
        rem = len - pos;
        if (o[pos] == '(') {
            if (bal > 0) rank += A089866_W[rem - 1][bal - 1];
            bal++;
        } else {
            bal--;
        }
    }
    return (seq_t)rank;
}
