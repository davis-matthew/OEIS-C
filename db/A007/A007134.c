/*
 * A007134  Number of connected labeled chordal graphs (or triangulated
 *          graphs) with n nodes.
 * https://oeis.org/A007134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Let Q_t(x) be the EGF of the chordal pieces C attached to a
 *            fixed t-clique T with C connected and N(C) = T. Let Z_t(x) =
 *            exp(Sum_{u=1..t} binomial(t,u)*Q_u(x)), the EGF of connected
 *            chordal graphs rooted at a t-clique. Applying the clique-tree
 *            dissymmetry identity #maxcliques - #separators = 1 gi...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t **Q_ = NULL, **Z_ = NULL, **ZQ_ = NULL, **F_ = NULL, **Cb_ = NULL;
static idx_t *cap_ = NULL;
static idx_t lcap_ = 0, S_ = 0;

static void grow_levels(idx_t need)
{
    idx_t nc, i;
    if (need < lcap_) return;
    nc = lcap_ * 2;
    if (nc < need + 1) nc = need + 1;
    if (nc < 8) nc = 8;
    Q_ = (seq_t **)realloc(Q_, nc * sizeof(seq_t *));
    Z_ = (seq_t **)realloc(Z_, nc * sizeof(seq_t *));
    ZQ_ = (seq_t **)realloc(ZQ_, nc * sizeof(seq_t *));
    F_ = (seq_t **)realloc(F_, nc * sizeof(seq_t *));
    Cb_ = (seq_t **)realloc(Cb_, nc * sizeof(seq_t *));
    cap_ = (idx_t *)realloc(cap_, nc * sizeof(idx_t));
    for (i = lcap_; i < nc; i++) {
        Q_[i] = NULL; Z_[i] = NULL; ZQ_[i] = NULL; F_[i] = NULL; Cb_[i] = NULL;
        cap_[i] = 0;
    }
    lcap_ = nc;
}

static void ensure(idx_t m, idx_t i)
{
    idx_t nc;
    if (i < cap_[m]) return;
    nc = cap_[m] * 2;
    if (nc < i + 1) nc = i + 1;
    if (nc < 4) nc = 4;
    if (cap_[m] == 0) {
        Q_[m] = alloc_zero(nc);
        Z_[m] = alloc_zero(nc);
        ZQ_[m] = alloc_zero(nc);
        F_[m] = alloc_zero(nc);
    } else {
        Q_[m] = grow_zero(Q_[m], cap_[m], nc);
        Z_[m] = grow_zero(Z_[m], cap_[m], nc);
        ZQ_[m] = grow_zero(ZQ_[m], cap_[m], nc);
        F_[m] = grow_zero(F_[m], cap_[m], nc);
    }
    cap_[m] = nc;
}

static void process_total(idx_t s)
{
    idx_t b, j, t, d, i, u, k;
    grow_levels(s);
    /* Pascal row s */
    Cb_[s] = alloc_zero(s + 1);
    Cb_[s][0] = 1;
    Cb_[s][s] = 1;
    for (b = 1; b < s; b++) Cb_[s][b] = Cb_[s - 1][b - 1] + Cb_[s - 1][b];
    /* new level s at degree 0 */
    ensure(s, 0);
    Q_[s][0] = 0; F_[s][0] = 0; Z_[s][0] = 1; ZQ_[s][0] = 0;
    for (j = 1; j < s; j++) {
        seq_t P = 0, M = 0, S2 = 0, f = 0, z = 0;
        t = s - j;
        ensure(t, j);
        for (d = 1; d <= j; d++) {
            P += Cb_[j][d] * Z_[t + d][j - d];
            M += Cb_[j][d] * ZQ_[t + d][j - d];
        }
        for (i = 1; i < j; i++) S2 += Cb_[j][i] * Z_[t][i] * Q_[t][j - i];
        M += S2;
        Q_[t][j] = P - M;
        for (u = 1; u <= t; u++) f += Cb_[t][u] * Q_[u][j];
        F_[t][j] = f;
        for (k = 1; k <= j; k++) z += Cb_[j - 1][k - 1] * F_[t][k] * Z_[t][j - k];
        Z_[t][j] = z;
        ZQ_[t][j] = Q_[t][j] + S2;
    }
}

seq_t a(idx_t n)
{
    idx_t s;
    if (S_ == 0) {
        grow_levels(2);
        Cb_[0] = alloc_zero(1);
        Cb_[0][0] = 1;
        Cb_[1] = alloc_zero(2);
        Cb_[1][0] = 1;
        Cb_[1][1] = 1;
        ensure(1, 0);
        Q_[1][0] = 0; F_[1][0] = 0; Z_[1][0] = 1; ZQ_[1][0] = 0;
        S_ = 1;
    }
    if (n < 1) return 0;
    for (s = S_ + 1; s <= n; s++) {
        process_total(s);
        S_ = s;
    }
    return Z_[1][n - 1];
}
