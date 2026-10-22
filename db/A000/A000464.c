/*
 * A000464  Expansion of e.g.f. sin(x)/cos(2*x).
 * https://oeis.org/A000464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = A001586(2n+1), since sin(x)/cos(2x) = (S(x) - S(-x))/2
 *            with S(x) = 1/(cos(x) - sin(x)). S(x) = sec(x + Pi/4)/sqrt(2),
 *            so A001586(m) = P_m(1), where P_0(t) = 1 and P_{m+1}(t) = (1 +
 *            t^2)*P_m'(t) + t*P_m(t) (all coefficients nonnegative)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *pc = NULL;     /* coefficients of P_mcur */
static seq_t *pd = NULL;     /* scratch */
static seq_t *vals = NULL;   /* vals[k] = a(k) */
static idx_t cap = 0;        /* capacity of pc, pd */
static idx_t vcap = 0;       /* capacity of vals */
static idx_t mcur = 0;
static int inited = 0;

static void ensure_cap(idx_t need)
{
    idx_t newcap, i, nv;
    seq_t *c2, *d2, *v2;
    if (need <= cap) return;
    newcap = 2 * need;
    if (newcap < 64) newcap = 64;
    nv = newcap / 2 + 2;
    c2 = alloc_zero(newcap);
    d2 = alloc_zero(newcap);
    v2 = alloc_zero(nv);
    if (inited) {
        for (i = 0; i <= mcur && i < cap; i++) c2[i] = pc[i];
        for (i = 0; i < vcap; i++) v2[i] = vals[i];
    }
    pc = c2;
    pd = d2;
    vals = v2;
    cap = newcap;
    vcap = nv;
}

seq_t a(idx_t n)
{
    idx_t target = 2 * n + 1;
    idx_t i;
    ensure_cap(target + 4);
    if (!inited) {
        pc[0] = 1;
        mcur = 0;
        inited = 1;
    }
    while (mcur < target) {
        idx_t m = mcur;
        seq_t *tmp;
        for (i = 0; i <= m + 1; i++) {
            seq_t v = pc[i + 1] * (seq_t)(i + 1);
            if (i > 0) v += pc[i - 1] * (seq_t)i;
            pd[i] = v;
        }
        tmp = pc;
        pc = pd;
        pd = tmp;
        mcur = m + 1;
        if (mcur & 1) {
            seq_t s = 0;
            for (i = 0; i <= mcur; i++) s += pc[i];
            vals[(mcur - 1) / 2] = s;
        }
    }
    return vals[n];
}
