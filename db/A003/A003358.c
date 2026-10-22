/*
 * A003358  Numbers that are the sum of 2 nonzero 6th powers.
 * https://oeis.org/A003358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - merge rows i^6 + j^6 (1 <= i <= j) in increasing order,
 *            dropping duplicates
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 785, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   s6_pow6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/s6_pow6.h"

#define S6_MAXR 65536

static seq_t s6_rv[S6_MAXR];
static unsigned char s6_rinf[S6_MAXR];
static idx_t s6_rj[S6_MAXR];
static idx_t s6_R = 0;

static uint32_t *s6_ti = NULL, *s6_tj = NULL;
static idx_t s6_Tn = 0, s6_Tcap = 0;
static seq_t s6_last;

static void s6_set_val(idx_t r) {
    int o1, o2;
    seq_t p = s6_pow6(r + 1, &o1);
    seq_t q = s6_pow6(s6_rj[r], &o2);
    seq_t s;
    if (o1 || o2) { s6_rinf[r] = 1; return; }
    s = p + q;
    if (s < p) { s6_rinf[r] = 1; return; }
    s6_rv[r] = s;
    s6_rinf[r] = 0;
}

static void s6_add_row(void) {
    if (s6_R >= S6_MAXR) return;
    s6_rj[s6_R] = s6_R + 1;
    s6_R++;
    s6_set_val(s6_R - 1);
}

static void s6_push(uint32_t i, uint32_t j) {
    if (s6_Tn >= s6_Tcap) {
        idx_t nc = s6_Tcap ? s6_Tcap * 2 : 256;
        s6_ti = (uint32_t *)realloc(s6_ti, nc * sizeof(uint32_t));
        s6_tj = (uint32_t *)realloc(s6_tj, nc * sizeof(uint32_t));
        s6_Tcap = nc;
    }
    s6_ti[s6_Tn] = i;
    s6_tj[s6_Tn] = j;
    s6_Tn++;
}

seq_t a(idx_t n) {
    int o;
    if (s6_R == 0) s6_add_row();
    while (s6_Tn < n) {
        idx_t r, best = s6_R;
        seq_t v;
        for (r = 0; r < s6_R; r++) {
            if (s6_rinf[r]) continue;
            if (best == s6_R || s6_rv[r] < s6_rv[best]) best = r;
        }
        if (best == s6_R) return 0; /* remaining terms exceed seq_t */
        v = s6_rv[best];
        if (s6_Tn == 0 || v != s6_last) {
            s6_push((uint32_t)(best + 1), (uint32_t)s6_rj[best]);
            s6_last = v;
        }
        /* last row still at its diagonal: the next row may now be needed */
        if (!s6_rinf[s6_R - 1] && s6_rj[s6_R - 1] == s6_R && s6_rv[s6_R - 1] == v)
            s6_add_row();
        for (r = 0; r < s6_R; r++) {
            if (!s6_rinf[r] && s6_rv[r] == v) {
                s6_rj[r]++;
                s6_set_val(r);
            }
        }
    }
    return s6_pow6(s6_ti[n - 1], &o) + s6_pow6(s6_tj[n - 1], &o);
}
