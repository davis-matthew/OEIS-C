/*
 * A069776  Permutation of natural numbers induced by the automorphism
 *          gma069776! acting on the parenthesizations encoded by A014486.
 * https://oeis.org/A069776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486-rank of f(T_n), where T_n is the binary tree
 *            encoded by A014486(n) and f is defined recursively on the car
 *            only: f(nil) = nil, f((nil . B)) = (nil . B), and with f(A) = (x
 *            . y) for nonempty A: f((A . B)) = ((x . B) . y). Dyck words are
 *            ranked and unranked by counting lattice paths
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A69776_K 32
#define A69776_MAXN 80

static uint64_t a69776_cnt[2 * A69776_K + 2][2 * A69776_K + 3];
static uint64_t a69776_off[A69776_K + 2];
static bool a69776_ready = false;

static int a69776_L[4 * A69776_MAXN];
static int a69776_R[4 * A69776_MAXN];
static int a69776_nn;
static char a69776_w[2 * A69776_K + 2];
static int a69776_pos;
static int a69776_len;

static void a69776_init(void)
{
    int r, h, k;
    for (r = 0; r <= 2 * A69776_K; r++)
        for (h = 0; h <= 2 * A69776_K + 1; h++)
            a69776_cnt[r][h] = 0;
    a69776_cnt[0][0] = 1;
    for (r = 1; r <= 2 * A69776_K; r++) {
        for (h = 0; h <= 2 * A69776_K; h++) {
            uint64_t v = a69776_cnt[r - 1][h + 1];
            if (h > 0) v += a69776_cnt[r - 1][h - 1];
            a69776_cnt[r][h] = v;
        }
    }
    a69776_off[0] = 0;
    for (k = 0; k <= A69776_K; k++)
        a69776_off[k + 1] = a69776_off[k] + a69776_cnt[2 * k][0];
    a69776_ready = true;
}

static int a69776_mk(int l, int r)
{
    a69776_L[a69776_nn] = l;
    a69776_R[a69776_nn] = r;
    return a69776_nn++;
}

static int a69776_parse(void)
{
    int A, B;
    if (a69776_pos >= a69776_len || a69776_w[a69776_pos] == '0')
        return -1;
    a69776_pos++;
    A = a69776_parse();
    a69776_pos++; /* closing */
    B = a69776_parse();
    return a69776_mk(A, B);
}

/* f((A . B)) = sigma((f(A) . B)), sigma(((x . y) . B)) = ((x . B) . y) */
static int a69776_g(int s)
{
    int A, B;
    if (s < 0) return -1;
    A = a69776_g(a69776_L[s]);
    B = a69776_R[s];
    if (A >= 0) {
        int X = a69776_L[A], Y = a69776_R[A];
        int inner = a69776_mk(X, B);
        return a69776_mk(inner, Y);
    }
    return a69776_mk(-1, B);
}

static void a69776_ser(int s)
{
    while (s >= 0) {
        a69776_w[a69776_len++] = '1';
        a69776_ser(a69776_L[s]);
        a69776_w[a69776_len++] = '0';
        s = a69776_R[s];
    }
}

seq_t a(idx_t n)
{
    int k, i, h, root, g;
    uint64_t r, rank;
    uint64_t m = (uint64_t)n;

    if (!a69776_ready) a69776_init();

    k = 0;
    while (k < A69776_K && a69776_off[k + 1] <= m) k++;
    r = m - a69776_off[k];

    /* unrank */
    h = 0;
    a69776_len = 2 * k;
    for (i = 0; i < 2 * k; i++) {
        int rem = 2 * k - i - 1;
        uint64_t c = 0;
        if (h > 0) c = a69776_cnt[rem][h - 1];
        if (h > 0 && r < c) {
            a69776_w[i] = '0';
            h--;
        } else {
            r -= c;
            a69776_w[i] = '1';
            h++;
        }
    }

    a69776_nn = 0;
    a69776_pos = 0;
    root = a69776_parse();
    g = a69776_g(root);
    a69776_len = 0;
    a69776_ser(g);

    /* rank */
    rank = 0;
    h = 0;
    for (i = 0; i < 2 * k; i++) {
        int rem = 2 * k - i - 1;
        if (a69776_w[i] == '0') {
            h--;
        } else {
            if (h > 0) rank += a69776_cnt[rem][h - 1];
            h++;
        }
    }
    return (seq_t)(a69776_off[k] + rank);
}
