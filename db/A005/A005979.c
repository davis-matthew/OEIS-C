/*
 * A005979  Number of Hamiltonian rooted triangulations with n internal nodes
 *          and 5 external nodes.
 * https://oeis.org/A005979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^(n+5)] H(t)*psi'(t)*(1/S(t))^(n+6) (Lagrange
 *            inversion), where S(t) = Sum_{i>=0} C(i)^2 t^i (C = Catalan
 *            numbers), psi(t) = t*S(t), H(t) = Sum_{p>=5} C(p-2) * [x^(p-5)]
 *            c(x)^5 * t^p, c(x) = Sum C(i) x^i. Derivation: a rooted 5-gon
 *            triangulation with a Hamiltonian cycle on m = n+5 vertic...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static bool fw;

/* (*v,*ng) += (x, xn); in fixed-width mode values simply wrap (ring arithmetic). */
static void acc(seq_t *v, bool *ng, seq_t x, bool xn) {
    if (fw) {
        if (xn) *v -= x; else *v += x;
        return;
    }
    if (*ng == xn) *v += x;
    else if (*v >= x) *v -= x;
    else { *v = x - *v; *ng = xn; }
}

static void sneg(seq_t *v, bool *ng) {
    if (fw) *v = (seq_t)0 - *v;
    else *ng = !*ng;
}

static void smul(const seq_t *av, const bool *an, const seq_t *bv, const bool *bn,
                 seq_t *cv, bool *cn, idx_t L) {
    idx_t k, i;
    for (k = 0; k < L; k++) {
        seq_t s = 0;
        bool sn = false;
        for (i = 0; i <= k; i++) {
            if (av[i] == 0 || bv[k - i] == 0) continue;
            acc(&s, &sn, av[i] * bv[k - i], an[i] != bn[k - i]);
        }
        cv[k] = s;
        cn[k] = sn;
    }
}

seq_t a(idx_t n) {
    idx_t m = n + 5, L = m + 1, i, j, k, r;
    seq_t *cat, *S, *U, *R, *Bs, *Tm, *P, *P2, *H, *swp, res;
    bool *Z, *Un, *Rn, *Bn, *Tn, *bswp, resn;
    idx_t e;

    fw = seq_is_fixed_width();

    cat = alloc_zero(L);
    S = alloc_zero(L);
    U = alloc_zero(L);
    R = alloc_zero(L);
    Bs = alloc_zero(L);
    Tm = alloc_zero(L);
    P = alloc_zero(L);
    P2 = alloc_zero(L);
    H = alloc_zero(L);
    Z = (bool *)calloc(L, sizeof(bool));
    Un = (bool *)calloc(L, sizeof(bool));
    Rn = (bool *)calloc(L, sizeof(bool));
    Bn = (bool *)calloc(L, sizeof(bool));
    Tn = (bool *)calloc(L, sizeof(bool));

    /* Catalan numbers by convolution */
    cat[0] = 1;
    for (i = 1; i <= m; i++) {
        seq_t s = 0;
        for (j = 0; j < i; j++) s += cat[j] * cat[i - 1 - j];
        cat[i] = s;
    }
    for (i = 0; i <= m; i++) S[i] = cat[i] * cat[i];

    /* U = 1/S */
    U[0] = 1;
    for (k = 1; k <= m; k++) {
        seq_t s = 0;
        bool sn = false;
        for (j = 1; j <= k; j++) {
            if (U[k - j] == 0) continue;
            acc(&s, &sn, S[j] * U[k - j], Un[k - j]);
        }
        sneg(&s, &sn);
        U[k] = s;
        Un[k] = sn;
    }

    /* R = U^(m+1) truncated to degree m */
    R[0] = 1;
    for (i = 0; i < L; i++) { Bs[i] = U[i]; Bn[i] = Un[i]; }
    e = m + 1;
    while (e) {
        if (e & 1) {
            smul(R, Rn, Bs, Bn, Tm, Tn, L);
            swp = R; R = Tm; Tm = swp;
            bswp = Rn; Rn = Tn; Tn = bswp;
        }
        e >>= 1;
        if (e) {
            smul(Bs, Bn, Bs, Bn, Tm, Tn, L);
            swp = Bs; Bs = Tm; Tm = swp;
            bswp = Bn; Bn = Tn; Tn = bswp;
        }
    }

    /* P = c(x)^5 */
    for (i = 0; i < L; i++) P[i] = cat[i];
    for (k = 0; k < 4; k++) {
        smul(P, Z, cat, Z, P2, Tn, L);
        swp = P; P = P2; P2 = swp;
    }

    /* H_p = C(p-2) * [x^(p-5)] c^5 */
    for (i = 5; i <= m; i++) H[i] = cat[i - 2] * P[i - 5];

    /* a = Sum_{r} (Sum_i H_i * (r-i+1) S_{r-i}) * R_{m-r} */
    res = 0;
    resn = false;
    for (r = 5; r <= m; r++) {
        seq_t q = 0;
        if (R[m - r] == 0) continue;
        for (i = 5; i <= r; i++)
            q += H[i] * ((seq_t)(r - i + 1) * S[r - i]);
        acc(&res, &resn, q * R[m - r], Rn[m - r]);
    }

    release(cat, L);
    release(S, L);
    release(U, L);
    release(R, L);
    release(Bs, L);
    release(Tm, L);
    release(P, L);
    release(P2, L);
    release(H, L);
    free(Z);
    free(Un);
    free(Rn);
    free(Bn);
    free(Tn);

    return res;
}
