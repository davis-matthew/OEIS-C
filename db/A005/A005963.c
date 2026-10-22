/*
 * A005963  Number of restricted hexagonal polyominoes with n cells.
 * https://oeis.org/A005963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000228(n) - A002216(n). The free polyhexes A000228(n)
 *            are found by a search: Redelmeier enumeration of fixed
 *            polyhexes, each weighted by its symmetry count and the total
 *            divided by 12. The tree-like (catacondensed) polyhexes
 *            A002216(n) come from Burnside's lemma over D6, using P(x) =
 *            x*(1...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static int N_, W_, bufsz_;
static unsigned char *seen_ = NULL, *occ_ = NULL;
static int *bufs_ = NULL;
static int *cq_ = NULL, *cr_ = NULL;
static int off_[6];
static uint64_t *res_ = NULL;
static int done_ = 0;

static const int perm_[6][3] = {
    {0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0, 2, 1}, {2, 1, 0}, {1, 0, 2}
};

static int stabilizer(int k) {
    int lo[3], hi[3], w[3], c[3];
    int i, j, g, sgi, cnt = 1;
    lo[0] = hi[0] = cq_[0];
    lo[1] = hi[1] = cr_[0];
    lo[2] = hi[2] = -cq_[0] - cr_[0];
    for (i = 1; i < k; i++) {
        c[0] = cq_[i]; c[1] = cr_[i]; c[2] = -c[0] - c[1];
        for (j = 0; j < 3; j++) {
            if (c[j] < lo[j]) lo[j] = c[j];
            if (c[j] > hi[j]) hi[j] = c[j];
        }
    }
    for (i = 0; i < 3; i++) w[i] = hi[i] - lo[i];
    for (g = 0; g < 6; g++) {
        const int *p = perm_[g];
        if (w[p[0]] != w[0] || w[p[1]] != w[1] || w[p[2]] != w[2]) continue;
        for (sgi = 0; sgi < 2; sgi++) {
            int sg = sgi ? -1 : 1;
            int mq = 0, mr = 0, ok = 1;
            if (g == 0 && sg == 1) continue;
            for (i = 0; i < k; i++) {
                int q2, r2;
                c[0] = cq_[i]; c[1] = cr_[i]; c[2] = -c[0] - c[1];
                q2 = sg * c[p[0]]; r2 = sg * c[p[1]];
                if (i == 0 || r2 < mr || (r2 == mr && q2 < mq)) { mq = q2; mr = r2; }
            }
            for (i = 0; i < k && ok; i++) {
                int dq, dr;
                c[0] = cq_[i]; c[1] = cr_[i]; c[2] = -c[0] - c[1];
                dq = sg * c[p[0]] - mq; dr = sg * c[p[1]] - mr;
                if (dr < 0 || dr > N_ || dq < -N_ || dq > N_) { ok = 0; break; }
                if (!occ_[(dr + 1) * W_ + dq + N_ + 1]) ok = 0;
            }
            if (ok) cnt++;
        }
    }
    return cnt;
}

static void rec(int depth, int *u, int nu) {
    int *v = bufs_ + (size_t)(depth + 1) * bufsz_;
    while (nu > 0) {
        int c = u[--nu];
        occ_[c] = 1;
        cq_[depth] = c % W_ - N_ - 1;
        cr_[depth] = c / W_ - 1;
        res_[depth + 1] += (uint64_t)stabilizer(depth + 1);
        if (depth + 1 < N_) {
            int nv = nu, na = 0, d, added[6];
            memcpy(v, u, (size_t)nu * sizeof(int));
            for (d = 0; d < 6; d++) {
                int x = c + off_[d];
                if (!seen_[x]) { seen_[x] = 1; v[nv++] = x; added[na++] = x; }
            }
            rec(depth + 1, v, nv);
            for (d = 0; d < na; d++) seen_[added[d]] = 0;
        }
        occ_[c] = 0;
    }
}

static void run(int n) {
    int H, q, o;
    N_ = n;
    W_ = 2 * n + 3;
    H = n + 3;
    bufsz_ = 6 * n + 8;
    free(seen_); free(occ_); free(bufs_); free(cq_); free(cr_); free(res_);
    seen_ = (unsigned char *)calloc((size_t)W_ * H, 1);
    occ_ = (unsigned char *)calloc((size_t)W_ * H, 1);
    bufs_ = (int *)malloc((size_t)(n + 2) * bufsz_ * sizeof(int));
    cq_ = (int *)malloc((size_t)(n + 1) * sizeof(int));
    cr_ = (int *)malloc((size_t)(n + 1) * sizeof(int));
    res_ = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    /* directions in cyclic order around a hexagon (axial coords) */
    off_[0] = 1;
    off_[1] = 1 - W_;
    off_[2] = -W_;
    off_[3] = -1;
    off_[4] = -1 + W_;
    off_[5] = W_;
    for (q = 0; q < W_; q++) seen_[q] = 1;                    /* r = -1 */
    for (q = -(n + 1); q < 0; q++) seen_[W_ + q + n + 1] = 1; /* r = 0, q < 0 */
    o = W_ + n + 1;
    seen_[o] = 1;
    bufs_[0] = o;
    rec(0, bufs_, 1);
    done_ = n;
}

/* tree-like (catacondensed, helicenes allowed) polyhexes, by Burnside over D6 */
static u128_t catacondensed(int n) {
    int i, j, k;
    u128_t *p = (u128_t *)calloc((size_t)n + 2, sizeof(u128_t));
    u128_t *p2 = (u128_t *)calloc((size_t)n + 2, sizeof(u128_t));
    u128_t *e = (u128_t *)calloc((size_t)n + 2, sizeof(u128_t));
    u128_t *e2 = (u128_t *)calloc((size_t)n + 2, sizeof(u128_t));
    u128_t R, p3, tot, fa, fb, r180, r120, r60;
    p[1] = 1;
    for (i = 2; i <= n; i++) {
        u128_t s = 3 * p[i - 1];
        for (j = 1; j < i - 1; j++) s += p[j] * p[i - 1 - j];
        p[i] = s;
    }
    for (i = 0; i <= n; i++) {
        u128_t s = 0;
        for (j = 1; j < i; j++) s += p[j] * p[i - j];
        p2[i] = s;
    }
    p3 = 0;
    for (j = 1; j < n - 1; j++) p3 += p[j] * p2[n - 1 - j];
    R = (n == 1 ? 1 : 0) + 6 * p[n - 1] + 9 * p2[n - 1] + 2 * p3;
    tot = R / (u128_t)n;
    r60 = (n == 1) ? 1 : 0;
    r120 = (n == 1) ? 1 : (((n - 1) % 3 == 0) ? 2 * p[(n - 1) / 3] : 0);
    if (n == 1) r180 = 1;
    else if (n % 2) r180 = 3 * p[(n - 1) / 2];
    else r180 = 3 * p[n / 2];
    if (n == 1) fb = 1;
    else if (n % 2) fb = p[(n - 1) / 2];
    else fb = p[n / 2];
    e[0] = 1;
    for (i = 1; 2 * i <= n; i++) e[2 * i] = p[i];
    for (i = 0; i <= n; i++) {
        u128_t s = 0;
        for (j = 0; j <= i; j++) s += e[j] * e[i - j];
        e2[i] = s;
    }
    if (n == 1) fa = 1;
    else if (n % 2) fa = 2 * p[(n - 1) / 2];
    else fa = 0;
    for (k = 2; k <= n; k++) fa += e2[n - k];
    tot += 2 * r60 + 2 * r120 + r180 + 3 * fa + 3 * fb;
    free(p); free(p2); free(e); free(e2);
    return tot / 12;
}

seq_t a(idx_t n) {
    u128_t total;
    if (n < 1) return 0;
    if ((int)n > done_) run((int)n);
    total = (u128_t)(res_[n] / 12);
    return u128_to_seq(total - catacondensed((int)n));
}
