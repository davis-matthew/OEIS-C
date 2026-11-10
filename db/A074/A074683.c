/*
 * A074683  Permutation of natural numbers induced by the Catalan Automorphism
 *          *A074683 acting on parenthesizations as encoded and ordered by
 *          A014486/A063171.
 * https://oeis.org/A074683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base + rank, where the n-th Dyck word of A014486 (word =
 *            1 L 0 R) is read as a binary tree. Its preorder code (0 leaf, 1
 *            left child only, 2 right child only, 3 both, final 0 dropped) is
 *            ranked among all same-size codes in the A085184 order
 *            (increasing digits), and base is the sum of Catala...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

typedef unsigned __int128 a074683_u128;

static a074683_u128 a074683_D[82][83];
static a074683_u128 a074683_G[41][43];
static int a074683_ready = 0;
static int a074683_w[96];
static int a074683_mt[96];
static int a074683_dig[48];
static int a074683_nd;
static const int a074683_wt[4] = { -1, 0, 0, 1 };

static void a074683_init(void)
{
    int r, b, l;
    if (a074683_ready) return;
    a074683_D[0][0] = 1;
    for (r = 1; r <= 81; ++r)
        for (b = 0; b <= 81; ++b) {
            a074683_a074683_dummy:;
            a074683_u128 v = a074683_D[r - 1][b + 1];
            if (b > 0) v += a074683_D[r - 1][b - 1];
            a074683_D[r][b] = v;
        }
    a074683_G[0][0] = 1;
    for (l = 1; l <= 40; ++l)
        for (b = 0; b <= 41; ++b) {
            a074683_u128 v = a074683_G[l - 1][b + 1] + a074683_G[l - 1][b] * 2;
            if (b > 0) v += a074683_G[l - 1][b - 1];
            a074683_G[l][b] = v;
        }
    a074683_ready = 1;
}

static void a074683_enc(int lo, int hi)
{
    int m, dgt;
    if (lo >= hi) return;
    m = a074683_mt[lo];
    dgt = ((m > lo + 1) ? 1 : 0) + ((hi > m + 1) ? 2 : 0);
    a074683_dig[a074683_nd++] = dgt;
    a074683_enc(lo + 1, m);
    a074683_enc(m + 1, hi);
}

static seq_t a074683_conv(a074683_u128 v)
{
    uint64_t hi = (uint64_t)(v >> 64);
    uint64_t lo = (uint64_t)v;
    seq_t r = (seq_t)0;
    if (hi != 0) {
        r = (seq_t)hi;
        r = r * (seq_t)4294967296ULL;
        r = r * (seq_t)4294967296ULL;
    }
    r = r + (seq_t)lo;
    return r;
}

seq_t a(idx_t n)
{
    a074683_u128 idx = (a074683_u128)n, base = 0, cat = 0, rank = 0;
    int k = 0, pos, bal, i, d, L, top;
    int stk[96];

    a074683_init();
    for (k = 0; k < 40; ++k) {
        cat = a074683_D[2 * k][0];
        if (idx < cat) break;
        idx -= cat;
        base += cat;
    }
    if (k <= 1) return a074683_conv(base);

    /* unrank Dyck word (1 = up) of k pairs, increasing binary order */
    bal = 0;
    for (pos = 0; pos < 2 * k; ++pos) {
        int rem = 2 * k - pos;
        a074683_u128 c0 = (bal > 0) ? a074683_D[rem - 1][bal - 1] : 0;
        if (idx < c0) { a074683_w[pos] = 0; bal--; }
        else { idx -= c0; a074683_w[pos] = 1; bal++; }
    }
    /* matching */
    top = 0;
    for (i = 0; i < 2 * k; ++i) {
        if (a074683_w[i]) stk[top++] = i;
        else { int o = stk[--top]; a074683_mt[o] = i; }
    }
    a074683_nd = 0;
    a074683_enc(0, 2 * k);
    L = a074683_nd - 1;

    bal = 0;
    for (pos = 0; pos < L; ++pos) {
        int rem = L - pos - 1;
        for (d = 0; d < a074683_dig[pos]; ++d) {
            int nb = bal + a074683_wt[d];
            if (nb >= 0 && nb <= 41) rank += a074683_G[rem][nb];
        }
        bal += a074683_wt[a074683_dig[pos]];
    }
    return a074683_conv(base + rank);
}
