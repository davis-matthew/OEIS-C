/*
 * A003437  Number of unlabeled Hamiltonian circuits on n-octahedron (cross
 *          polytope); also number of circular chord diagrams with n chords,
 *          modulo symmetries.
 * https://oeis.org/A003437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Burnside's lemma over the dihedral group of the 2n-gon, with
 *            inclusion-exclusion over the forbidden adjacent chords: a(n) = (
 *            Sum_{d|2n} phi(2n/d)*(p_k(d) - p_k(d-2)) + n*(p_2(n) - p_2(n-1)
 *            + p_2(n-2)) ) / (4n), where k = 2n/d, e = [k even], and p_k(L) =
 *            Sum_j (-1)^j C(L-j,j) F_k(L-2j) with F_k(m...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a3437_fixed, a3437_phi, a3437_sseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/a3437_fixed.h"
#include "../utils/a3437_phi.h"
#include "../utils/a3437_sseq.h"

typedef struct { uint32_t l[8]; } a3437_w;

static void a3437_wadd(a3437_w *r, const a3437_w *a, const a3437_w *b) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < 8; i++) {
        c += (uint64_t)a->l[i] + b->l[i];
        r->l[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a3437_wsub(a3437_w *r, const a3437_w *a, const a3437_w *b) {
    uint64_t br = 0;
    int i;
    for (i = 0; i < 8; i++) {
        uint64_t t = (uint64_t)a->l[i] - (uint64_t)b->l[i] - br;
        r->l[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

static void a3437_wmul(a3437_w *r, const a3437_w *a, uint64_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < 8; i++) {
        c += (uint64_t)a->l[i] * m;
        r->l[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a3437_wdiv(a3437_w *a, uint64_t d) {
    uint64_t r = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        uint64_t cur = (r << 32) | a->l[i];
        a->l[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

/* fixed-width: exact 256-bit p-sequence; o[0]=p(D), o[1]=p(D-1), o[2]=p(D-2) */
static void a3437_wseq(idx_t D, idx_t k, a3437_w *o) {
    a3437_w w[6], pos, neg, t;
    int e = (k % 2 == 0);
    idx_t L;
    int i;
    memset(w, 0, sizeof w);
    w[0].l[0] = 1;
    for (L = 1; L <= D; L++) {
        memset(&pos, 0, sizeof pos);
        memset(&neg, 0, sizeof neg);
        if (e) {
            a3437_wadd(&pos, &pos, &w[0]);
            a3437_wadd(&neg, &neg, &w[4]);
        }
        if (L >= 2) {
            a3437_wmul(&t, &w[1], (uint64_t)(k * (L - 1) - 1));
            a3437_wadd(&pos, &pos, &t);
        }
        if (L >= 4) {
            a3437_wmul(&t, &w[3], (uint64_t)(k * (L - 3) + 1));
            a3437_wadd(&pos, &pos, &t);
        }
        a3437_wadd(&pos, &pos, &w[5]);
        if (L == 4) {
            memset(&t, 0, sizeof t);
            t.l[0] = 1;
            a3437_wadd(&neg, &neg, &t);
        }
        for (i = 5; i >= 1; i--) w[i] = w[i - 1];
        a3437_wsub(&w[0], &pos, &neg);
    }
    o[0] = w[0];
    o[1] = w[1];
    o[2] = w[2];
}

seq_t a(idx_t n) {
    idx_t N, d;
    if (n < 2) return 0;
    N = 2 * n;
    if (a3437_fixed()) {
        a3437_w T, t, o[3];
        seq_t r = 0;
        int i, b;
        if (n > 40) return 0; /* value exceeds any fixed width */
        memset(&T, 0, sizeof T);
        for (d = 1; d <= N; d++) {
            idx_t k;
            if (N % d) continue;
            k = N / d;
            a3437_wseq(d, k, o);
            if (d == 1) t = o[0];
            else a3437_wsub(&t, &o[0], &o[2]);
            a3437_wmul(&t, &t, a3437_phi(k));
            a3437_wadd(&T, &T, &t);
        }
        a3437_wseq(n, 2, o);
        a3437_wadd(&t, &o[0], &o[2]);
        a3437_wsub(&t, &t, &o[1]);
        a3437_wmul(&t, &t, (uint64_t)n);
        a3437_wadd(&T, &T, &t);
        a3437_wdiv(&T, (uint64_t)(4 * n));
        for (i = 7; i >= 0; i--) {
            for (b = 3; b >= 0; b--) {
                r = (seq_t)(r * (seq_t)256);
                r = (seq_t)(r + (seq_t)((T.l[i] >> (8 * b)) & 255u));
            }
        }
        return r;
    } else {
        seq_t T = 0, o0, o1, o2;
        for (d = 1; d <= N; d++) {
            idx_t k;
            seq_t term;
            if (N % d) continue;
            k = N / d;
            a3437_sseq(d, k, &o0, &o1, &o2);
            if (d == 1) term = o0;
            else term = o0 - o2;
            T += term * (seq_t)a3437_phi(k);
        }
        a3437_sseq(n, 2, &o0, &o1, &o2);
        T += ((o0 + o2) - o1) * (seq_t)n;
        return T / ((seq_t)n * 4);
    }
}
