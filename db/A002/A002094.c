/*
 * A002094  Number of unlabeled connected loop-less graphs on n nodes
 *          containing exactly one cycle (of length at least 2) and with all
 *          nodes of degree <= 4.
 * https://oeis.org/A002094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with A(x) = 1 + x*(A(x)^3 + 3*A(x)*A(x^2) + 2*A(x^3))/6
 *            (A000598), B(x) = (A(x)^2 + A(x^2))/2 and C(x) = x*B(x) (one
 *            cycle vertex with its trees), G.f. = Sum_{k>=2} Z(D_k)(C), which
 *            evaluates to a(n) = ( 2*Sum_{d|n} phi(d)*P(n/d) + n*T(n) -
 *            4*n*C(n) ) / (4*n), where P(x) = x*C'(x)/(1-C(x)) and T(...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Helpers:   grow_zero, x2094_fixed, x2094_modrun, x2094_phi, x2094_pw,
 *            x2094_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/x2094_fixed.h"
#include "../utils/x2094_modrun.h"
#include "../utils/x2094_phi.h"
#include "../utils/x2094_pw.h"
#include "../utils/x2094_width.h"

static seq_t *gA, *gS2, *gA3, *gC, *gCC, *gT, *gP;
static idx_t gN = 0, gCap = 0;

static void x2094_extend(idx_t upto)
{
    idx_t m, i;
    if (upto < gN) return;
    if (upto + 1 > gCap) {
        idx_t nc = gCap * 2;
        if (nc < upto + 1) nc = upto + 1;
        if (nc < 64) nc = 64;
        gA = grow_zero(gA, gCap, nc);
        gS2 = grow_zero(gS2, gCap, nc);
        gA3 = grow_zero(gA3, gCap, nc);
        gC = grow_zero(gC, gCap, nc);
        gCC = grow_zero(gCC, gCap, nc);
        gT = grow_zero(gT, gCap, nc);
        gP = grow_zero(gP, gCap, nc);
        gCap = nc;
    }
    for (m = gN; m <= upto; m++) {
        if (m == 0) {
            gA[0] = 1; gS2[0] = 1; gA3[0] = 1;
            gC[0] = 0; gCC[0] = 0; gT[0] = 0; gP[0] = 0;
            continue;
        }
        {
            idx_t k = m - 1;
            seq_t s = gA3[k], t = 0, q;
            for (i = 0; 2 * i <= k; i++) t += gA[i] * gA[k - 2 * i];
            s += 3 * t;
            if (k % 3 == 0) s += 2 * gA[k / 3];
            gA[m] = s / 6;
            s = 0;
            for (i = 0; 2 * i < m; i++) s += gA[i] * gA[m - i];
            s *= 2;
            if (m % 2 == 0) s += gA[m / 2] * gA[m / 2];
            gS2[m] = s;
            s = 0;
            for (i = 0; i <= m; i++) s += gA[i] * gS2[m - i];
            gA3[m] = s;
            s = gS2[k];
            if (k % 2 == 0) s += gA[k / 2];
            gC[m] = s / 2;
            s = 0;
            for (i = 1; 2 * i < m; i++) s += gC[i] * gC[m - i];
            s *= 2;
            if (m % 2 == 0) s += gC[m / 2] * gC[m / 2];
            gCC[m] = s;
            q = 2 * gC[m] + gCC[m];
            if (m % 2 == 0) q += gC[m / 2];
            for (i = 1; 2 * i <= m; i++) q += gC[i] * gT[m - 2 * i];
            gT[m] = q;
            q = (seq_t)m * gC[m];
            for (i = 1; i <= m; i++) q += gC[i] * gP[m - i];
            gP[m] = q;
        }
    }
    gN = upto + 1;
}

seq_t a(idx_t n)
{
    int w = x2094_width();
    if (w < 400) {
        if (n > (idx_t)(2 * w + 40)) return 0; /* a(n) cannot fit in seq_t */
        return x2094_fixed(n);
    }
    {
        seq_t S = 0, num;
        idx_t d;
        x2094_extend(n);
        for (d = 1; d <= n; d++)
            if (n % d == 0) S += (seq_t)x2094_phi(d) * gP[n / d];
        num = 2 * S + (seq_t)n * gT[n];
        num -= (seq_t)n * 4 * gC[n];
        return num / ((seq_t)n * 4);
    }
}
