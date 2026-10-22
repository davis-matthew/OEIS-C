/*
 * A007836  Springer numbers associated with symplectic group.
 * https://oeis.org/A007836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = P_n(1) - Q_n(1), where d^n/dy^n tan(y) = P_n(tan y) and
 *            d^n/dy^n sec(y) = sec(y)*Q_n(tan y), i.e. P_0 = t, P_{n+1} =
 *            (1+t^2)*P_n'; Q_0 = 1, Q_{n+1} = (1+t^2)*Q_n' + t*Q_n (from
 *            e.g.f. (2*cos(x)-1)/(cos(x)-sin(x)) = 1 + tan(y) -
 *            sec(y)/sqrt(2), y = x+Pi/4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *Pc = 0, *Qc = 0, *Pn = 0, *Qn = 0, *res = 0;
static idx_t cap = 0;   /* length of each array */
static idx_t done = 0;  /* res[0..done-1] are valid; Pc,Qc hold P_{done-1},Q_{done-1} */

static void ensure_cap(idx_t len) {
    idx_t nc;
    if (len <= cap) return;
    nc = cap * 2;
    if (nc < len) nc = len;
    if (nc < 16) nc = 16;
    if (cap == 0) {
        Pc = alloc_zero(nc);
        Qc = alloc_zero(nc);
        Pn = alloc_zero(nc);
        Qn = alloc_zero(nc);
        res = alloc_zero(nc);
    } else {
        Pc = grow_zero(Pc, cap, nc);
        Qc = grow_zero(Qc, cap, nc);
        Pn = grow_zero(Pn, cap, nc);
        Qn = grow_zero(Qn, cap, nc);
        res = grow_zero(res, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    ensure_cap(n + 3);
    if (done == 0) {
        Pc[1] = 1;   /* P_0 = t */
        Qc[0] = 1;   /* Q_0 = 1 */
        res[0] = 1;
        done = 1;
    }
    while (done <= n) {
        idx_t m = done - 1;  /* current level: deg P = m+1, deg Q = m */
        idx_t j;
        seq_t sp = 0, sq = 0;
        seq_t *tmp;
        for (j = 0; j <= m + 2; j++) {
            seq_t v = 0;
            if (j + 1 <= m + 1) v += (seq_t)(j + 1) * Pc[j + 1];
            if (j >= 1 && j - 1 <= m + 1) v += (seq_t)(j - 1) * Pc[j - 1];
            Pn[j] = v;
            sp += v;
        }
        for (j = 0; j <= m + 1; j++) {
            seq_t w = 0;
            if (j + 1 <= m) w += (seq_t)(j + 1) * Qc[j + 1];
            if (j >= 1) w += (seq_t)j * Qc[j - 1];
            Qn[j] = w;
            sq += w;
        }
        tmp = Pc; Pc = Pn; Pn = tmp;
        tmp = Qc; Qc = Qn; Qn = tmp;
        res[m + 1] = sp - sq;
        done++;
    }
    return res[n];
}
