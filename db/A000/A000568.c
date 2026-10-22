/*
 * A000568  Number of outcomes of unlabeled n-team round-robin tournaments.
 * https://oeis.org/A000568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{j} 2^{t_j} / Product_k (k^(j_k) * j_k!), where j
 *            runs over the partitions of n into odd parts (j_k parts of size
 *            k) and t_j = (1/2)*[Sum_{r,s} j_r j_s gcd(r,s) - Sum_r j_r]
 *            (Davis's formula). The arbitrary-precision build sums (n!/D_j) *
 *            2^{t_j} exactly and then divides by n!. A fixed...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   a568_gcd, a568_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a568_gcd.h"
#include "../utils/a568_inv.h"

static int g568_fixed;
static idx_t *g568_P;
static idx_t *g568_M;
static unsigned __int128 g568_acc;
static seq_t g568_N;
static seq_t g568_big;

static void a568_add(uint64_t t, uint64_t d, unsigned __int128 dodd, seq_t D) {
    if (g568_fixed) {
        uint64_t e = t - d;
        if (e < 128) g568_acc += ((unsigned __int128)1 << e) * a568_inv(dodd);
    } else {
        g568_big += (g568_N / D) << t;
    }
}

static void a568_rec(idx_t rem, long long maxk, idx_t depth, uint64_t t, uint64_t d,
                     unsigned __int128 dodd, seq_t D) {
    long long k;
    if (rem == 0) {
        a568_add(t, d, dodd, D);
        return;
    }
    for (k = maxk; k >= 1; k -= 2) {
        idx_t uk = (idx_t)k;
        uint64_t cross = 0;
        unsigned __int128 dj = dodd;
        uint64_t dd = d;
        seq_t Dj = D;
        idx_t i, j;
        if (uk > rem) continue;
        for (i = 0; i < depth; i++) cross += a568_gcd(uk, g568_P[i]) * g568_M[i];
        for (j = 1; j * uk <= rem; j++) {
            idx_t jo = j;
            uint64_t tj;
            while (!(jo & 1)) {
                jo >>= 1;
                dd++;
            }
            dj *= (unsigned __int128)uk * jo;
            if (!g568_fixed) Dj *= (seq_t)uk * (seq_t)j;
            tj = t + j * cross + j * (j * uk - 1) / 2;
            g568_P[depth] = uk;
            g568_M[depth] = j;
            a568_rec(rem - j * uk, k - 2, depth + 1, tj, dd, dj, Dj);
        }
    }
}

seq_t a(idx_t n) {
    seq_t x = 1;
    seq_t r;
    idx_t i;
    long long maxk;
    for (i = 0; i < 130; i++) x += x;
    g568_fixed = (x == 0);

    g568_P = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    g568_M = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    g568_acc = 0;
    g568_big = 0;
    g568_N = 1;
    if (!g568_fixed) {
        for (i = 2; i <= n; i++) g568_N *= (seq_t)i;
    }

    maxk = (long long)n;
    if (maxk % 2 == 0) maxk--;
    a568_rec(n, maxk, 0, 0, 0, (unsigned __int128)1, (seq_t)1);

    free(g568_P);
    free(g568_M);

    if (!g568_fixed) return g568_big / g568_N;

    r = (seq_t)(uint64_t)(g568_acc >> 64);
    for (i = 0; i < 8; i++) r <<= 8;
    r += (seq_t)(uint64_t)g568_acc;
    return r;
}
