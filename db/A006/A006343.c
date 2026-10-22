/*
 * A006343  Arkons: number of elementary maps with n-1 nodes.
 * https://oeis.org/A006343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   O.g.f. A(x) = T(x)/x, where T = x*(1 + T^2*C(T)) and C(T) = 1 +
 *            T*C(T)^2, so a(n) = [x^(n+1)] T. With fixed-width seq_t this is
 *            computed only with + and *, so wraparound does not change the
 *            result. For arbitrary precision it uses the recurrence
 *            (n+3)*(n-1)*a(n) = (11*n^2-2*n-45)*a(n-1) - (37*n+29...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 65,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t mulw(seq_t x, seq_t y) { return (seq_t)(x * 1u * y); }

/* fixed-width: power series T, D = C(T), T^2, D^2 */
static seq_t *ST = NULL, *SD = NULL, *ST2 = NULL, *SD2 = NULL;
static idx_t fcap = 0, fcnt = 0;

static seq_t a_series(idx_t n) {
    idx_t need = n + 2, k, i;
    if (need > fcap) {
        idx_t nc = fcap * 2 > need ? fcap * 2 : need;
        ST = grow_zero(ST, fcap, nc);
        SD = grow_zero(SD, fcap, nc);
        ST2 = grow_zero(ST2, fcap, nc);
        SD2 = grow_zero(SD2, fcap, nc);
        fcap = nc;
    }
    for (k = fcnt; k < need; k++) {
        seq_t s = 0;
        for (i = 1; i < k; i++) s += mulw(ST[i], ST[k - i]);
        ST2[k] = s;
        if (k == 0) {
            ST[0] = 0;
        } else {
            seq_t p = 0;
            for (i = 0; i < k; i++) p += mulw(ST2[i], SD[k - 1 - i]);
            if (k == 1) p += 1;
            ST[k] = p;
        }
        {
            seq_t q = 0;
            for (i = 1; i <= k; i++) q += mulw(ST[i], SD2[k - i]);
            if (k == 0) q += 1;
            SD[k] = q;
        }
        s = 0;
        for (i = 0; i <= k; i++) s += mulw(SD[i], SD[k - i]);
        SD2[k] = s;
    }
    if (need > fcnt) fcnt = need;
    return ST[n + 1];
}

/* arbitrary precision: recurrence */
static seq_t *RC = NULL;
static idx_t rcap = 0, rcnt = 0;

static seq_t a_rec(idx_t n) {
    idx_t i;
    if (n + 1 > rcap) {
        idx_t nc = rcap * 2 > n + 1 ? rcap * 2 : n + 1;
        if (nc < 8) nc = 8;
        RC = grow_zero(RC, rcap, nc);
        rcap = nc;
    }
    if (rcnt == 0) {
        RC[0] = 1; RC[1] = 0; RC[2] = 1; RC[3] = 1; RC[4] = 4;
        rcnt = 5;
    }
    for (i = rcnt; i <= n; i++) {
        seq_t m = (seq_t)i;
        seq_t pos = (11 * m * m - 2 * m - 45) * RC[i - 1]
                  + (29 * m * m - 125 * m + 78) * RC[i - 3]
                  + (61 * m - 106) * (m - 3) * RC[i - 4];
        seq_t neg = (37 * m + 29) * (m - 3) * RC[i - 2]
                  + 155 * (m - 3) * (m - 4) * RC[i - 5];
        RC[i] = (pos - neg) / ((m + 3) * (m - 1));
    }
    if (n + 1 > rcnt) rcnt = n + 1;
    return RC[n];
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_series(n);
    return a_rec(n);
}
