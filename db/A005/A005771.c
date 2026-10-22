/*
 * A005771  Number of n-covers of an unlabeled 5-set.
 * https://oeis.org/A005771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052264(n) - A006148(n), where the number of n X k binary
 *            matrices up to row and column permutations is computed by
 *            Burnside over S_k: f_k(n) = (1/k!) * Sum_{sigma in S_k} [x^n]
 *            Prod_{cycles C of sigma on subsets} 1/(1-x^|C|), and a(n) =
 *            f_5(n) - f_4(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 45, u128 n <= 229,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

/* k, class size, number of cycles, cycle lengths */
static const int CLS[12][8] = {
    {5, 1, 5, 1, 1, 1, 1, 1}, {5, 10, 4, 2, 1, 1, 1, 0}, {5, 20, 3, 3, 1, 1, 0, 0},
    {5, 30, 2, 4, 1, 0, 0, 0}, {5, 24, 1, 5, 0, 0, 0, 0}, {5, 15, 3, 2, 2, 1, 0, 0},
    {5, 20, 2, 3, 2, 0, 0, 0},
    {4, 1, 4, 1, 1, 1, 1, 0}, {4, 6, 3, 2, 1, 1, 0, 0}, {4, 8, 2, 3, 1, 0, 0, 0},
    {4, 6, 1, 4, 0, 0, 0, 0}, {4, 3, 2, 2, 2, 0, 0, 0}
};

/* lengths of the cycles induced on the 2^k subsets by class ci */
static int subset_cycles(int ci, int *lens) {
    int k = CLS[ci][0], nc = CLS[ci][2];
    int p[5], pos = 0, i, t, cnt = 0;
    int seen[32] = {0};
    for (i = 0; i < nc; i++) {
        int L = CLS[ci][3 + i];
        for (t = 0; t < L; t++) p[pos + t] = pos + (t + 1) % L;
        pos += L;
    }
    for (i = 0; i < (1 << k); i++) {
        int m, len = 0;
        if (seen[i]) continue;
        m = i;
        do {
            int b, im = 0;
            seen[m] = 1;
            len++;
            for (b = 0; b < k; b++) if (m >> b & 1) im |= 1 << p[b];
            m = im;
        } while (m != i);
        lens[cnt++] = len;
    }
    return cnt;
}

static unsigned __int128 *fx_res = NULL;
static seq_t *bg_res = NULL;
static idx_t cap = 0;

static void compute_fixed(idx_t N) {
    unsigned __int128 *tot = (unsigned __int128 *)calloc(N + 1, sizeof(unsigned __int128));
    unsigned __int128 *c = (unsigned __int128 *)malloc((N + 1) * sizeof(unsigned __int128));
    unsigned __int128 inv = 15, mask = (((unsigned __int128)1) << 125) - 1;
    int ci, i, it, lens[32];
    idx_t j;
    for (it = 0; it < 6; it++) inv *= 2 - 15 * inv;
    for (ci = 0; ci < 12; ci++) {
        int nl = subset_cycles(ci, lens);
        unsigned __int128 w = (unsigned __int128)CLS[ci][1];
        if (CLS[ci][0] == 4) w = (unsigned __int128)0 - 5 * w;
        for (j = 0; j <= N; j++) c[j] = 0;
        c[0] = 1;
        for (i = 0; i < nl; i++) {
            idx_t L = (idx_t)lens[i];
            for (j = L; j <= N; j++) c[j] += c[j - L];
        }
        for (j = 0; j <= N; j++) tot[j] += w * c[j];
    }
    for (j = 0; j <= N; j++) tot[j] = ((tot[j] >> 3) * inv) & mask;
    free(c);
    free(fx_res);
    fx_res = tot;
}

static void compute_big(idx_t N) {
    seq_t *t5 = alloc_zero(N + 1), *t4 = alloc_zero(N + 1), *c = alloc_zero(N + 1);
    seq_t *res = alloc_zero(N + 1);
    int ci, i, lens[32];
    idx_t j;
    for (ci = 0; ci < 12; ci++) {
        int nl = subset_cycles(ci, lens);
        seq_t *tot = CLS[ci][0] == 5 ? t5 : t4;
        for (j = 0; j <= N; j++) c[j] = 0;
        c[0] = 1;
        for (i = 0; i < nl; i++) {
            idx_t L = (idx_t)lens[i];
            for (j = L; j <= N; j++) c[j] += c[j - L];
        }
        for (j = 0; j <= N; j++) tot[j] += c[j] * CLS[ci][1];
    }
    for (j = 0; j <= N; j++) res[j] = t5[j] / 120 - t4[j] / 24;
    release(t5, N + 1);
    release(t4, N + 1);
    release(c, N + 1);
    if (bg_res) release(bg_res, cap + 1);
    bg_res = res;
}

seq_t a(idx_t n) {
    if (n > cap || (fx_res == NULL && bg_res == NULL)) {
        idx_t N = n;
        if (N < 2 * cap) N = 2 * cap;
        if (N < 64) N = 64;
        if (seq_is_big()) compute_big(N);
        else compute_fixed(N);
        cap = N;
    }
    if (seq_is_big()) return bg_res[n];
    return u128_to_seq(fx_res[n]);
}
