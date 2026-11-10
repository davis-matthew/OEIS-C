/*
 * A226082  Number of daughters to wait before picking in the sultan's dowry
 *          problem that is not equal to round(n*e^(-1 - 1/(2*n))).
 * https://oeis.org/A226082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   search over m; m is a term iff round(m*e^(-1-1/(2m))) !=
 *            A054404(m), where A054404(m) = least r >= 1 with H(m-1) - H(r)
 *            <= 1, the harmonic numbers being evaluated (as in the OEIS data)
 *            by H(N) ~ log(N) + gamma + 1/(2N). Both sides are decided with
 *            double-double arithmetic
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

typedef struct { double hi, lo; } A226082_dd;

static A226082_dd A226082_mk(double h) { A226082_dd r; r.hi = h; r.lo = 0.0; return r; }

static A226082_dd A226082_qts(double a, double b) {
    A226082_dd r; double s = a + b; r.lo = b - (s - a); r.hi = s; return r;
}
static A226082_dd A226082_ts(double a, double b) {
    A226082_dd r; double s = a + b; double bb = s - a;
    r.lo = (a - (s - bb)) + (b - bb); r.hi = s; return r;
}
static void A226082_split(double a, double *h, double *l) {
    double t = 134217729.0 * a; *h = t - (t - a); *l = a - *h;
}
static A226082_dd A226082_tp(double a, double b) {
    A226082_dd r; double p = a * b, ah, al, bh, bl;
    A226082_split(a, &ah, &al); A226082_split(b, &bh, &bl);
    r.lo = ((ah * bh - p) + ah * bl + al * bh) + al * bl; r.hi = p; return r;
}
static A226082_dd A226082_add(A226082_dd a, A226082_dd b) {
    A226082_dd s = A226082_ts(a.hi, b.hi), t = A226082_ts(a.lo, b.lo);
    double e = s.lo + t.hi;
    s = A226082_qts(s.hi, e);
    e = s.lo + t.lo;
    return A226082_qts(s.hi, e);
}
static A226082_dd A226082_neg(A226082_dd a) { a.hi = -a.hi; a.lo = -a.lo; return a; }
static A226082_dd A226082_sub(A226082_dd a, A226082_dd b) { return A226082_add(a, A226082_neg(b)); }
static A226082_dd A226082_mul(A226082_dd a, A226082_dd b) {
    A226082_dd p = A226082_tp(a.hi, b.hi);
    double e = p.lo + (a.hi * b.lo + a.lo * b.hi);
    return A226082_qts(p.hi, e);
}
static A226082_dd A226082_div(A226082_dd a, A226082_dd b) {
    double q1 = a.hi / b.hi;
    A226082_dd r = A226082_sub(a, A226082_mul(A226082_mk(q1), b));
    double q2 = r.hi / b.hi;
    r = A226082_sub(r, A226082_mul(A226082_mk(q2), b));
    double q3 = r.hi / b.hi;
    A226082_dd q = A226082_qts(q1, q2);
    return A226082_add(q, A226082_mk(q3));
}

static A226082_dd A226082_inv_e;
static bool A226082_init_done = false;

static void A226082_init(void) {
    A226082_dd sum = A226082_mk(1.0), term = A226082_mk(1.0);
    for (int k = 1; k <= 32; ++k) {
        term = A226082_div(term, A226082_mk((double)k));
        if (k & 1) sum = A226082_sub(sum, term);
        else sum = A226082_add(sum, term);
    }
    A226082_inv_e = sum;
    A226082_init_done = true;
}

/* ln(a/b) - 1, computed as ln(1+t), t = (a/b)/e - 1, via 2*atanh(t/(2+t)) */
static A226082_dd A226082_lnratm1(double a, double b) {
    A226082_dd q = A226082_div(A226082_mk(a), A226082_mk(b));
    A226082_dd t = A226082_sub(A226082_mul(q, A226082_inv_e), A226082_mk(1.0));
    A226082_dd u = A226082_div(t, A226082_add(A226082_mk(2.0), t));
    A226082_dd u2 = A226082_mul(u, u);
    A226082_dd term = u, sum = u;
    for (int k = 3; k < 2000; k += 2) {
        term = A226082_mul(term, u2);
        A226082_dd c = A226082_div(term, A226082_mk((double)k));
        sum = A226082_add(sum, c);
        if (c.hi == 0.0 || fabs(c.hi) < 1e-34 * fabs(sum.hi)) break;
    }
    return A226082_add(sum, sum);
}

/* F(m,r) = [H(m-1) - H(r) - 1] with H(N) ~ ln N + gamma + 1/(2N):
   ln((m-1)/r) - 1 + 1/(2(m-1)) - 1/(2r) */
static double A226082_F(uint64_t m, uint64_t r) {
    double m1 = (double)(m - 1), rd = (double)r;
    A226082_dd res = A226082_lnratm1(m1, rd);
    res = A226082_add(res, A226082_div(A226082_mk(1.0), A226082_mk(2.0 * m1)));
    res = A226082_sub(res, A226082_div(A226082_mk(1.0), A226082_mk(2.0 * rd)));
    return res.hi;
}

static seq_t *A226082_found = NULL;
static idx_t A226082_cnt = 0, A226082_cap = 0;
static uint64_t A226082_cur = 2;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!A226082_init_done) A226082_init();
    while (A226082_cnt < n) {
        uint64_t m = A226082_cur++;
        double md = (double)m;
        double xd = md * exp(-1.0 - 0.5 / md);
        bool cand = (m < 2000);
        if (!cand) {
            double f = xd - floor(xd);
            double tol = 3.0 / md + 1e-15 * md;
            if (fabs(f - 0.5) < tol) cand = true;
        }
        if (!cand) continue;

        /* exact round(m*e^(-1-1/(2m))): x > R+1/2  <=>  ln(2m/(2R+1)) - 1 > 1/(2m) */
        A226082_dd h = A226082_div(A226082_mk(1.0), A226082_mk(2.0 * md));
        uint64_t R = (uint64_t)floor(xd + 0.5);
        while (A226082_sub(A226082_lnratm1(2.0 * md, 2.0 * (double)R + 1.0), h).hi > 0.0) R++;
        while (R > 0 && A226082_sub(A226082_lnratm1(2.0 * md, 2.0 * (double)R - 1.0), h).hi <= 0.0) R--;

        bool agree = false;
        if (R >= 1 && R <= m - 1 && A226082_F(m, R) <= 0.0) {
            if (R == 1 || A226082_F(m, R - 1) > 0.0) agree = true;
        }
        if (!agree) {
            if (A226082_cnt >= A226082_cap) {
                idx_t nc = A226082_cap ? A226082_cap * 2 : 64;
                if (!A226082_found) A226082_found = mem_new(seq_t, nc);
                else A226082_found = mem_resize(seq_t, A226082_found, nc);
                A226082_cap = nc;
            }
            A226082_found[A226082_cnt++] = (seq_t)m;
        }
    }
    return A226082_found[n - 1];
}
