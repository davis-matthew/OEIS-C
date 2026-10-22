/*
 * A005439  Genocchi medians (or Genocchi numbers of second kind).
 * https://oeis.org/A005439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = T(n, 1) where T(1, x) = 1; T(n, x) = (x+1)*((x+1)*T(n-1,
 *            x+1) - x*T(n-1, x)); a(0) = 1. Computed incrementally along
 *            antidiagonals k + x = s, memoized
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *res = 0;     /* res[m] = a(m), m < rlen */
static idx_t rlen = 0;
static seq_t *diag = 0;    /* diag[k] = T(k, s - k), 1 <= k < s */
static idx_t dcap = 0;
static idx_t s_cur = 0;
static idx_t rcap = 0;

static void ensure_diag(idx_t need) {
    if (need > dcap) {
        idx_t nc = dcap ? dcap : 16;
        while (nc < need) nc *= 2;
        diag = grow_zero(diag, dcap, nc);
        dcap = nc;
    }
}

static void ensure_res(idx_t need) {
    if (need > rcap) {
        idx_t nc = rcap ? rcap : 16;
        while (nc < need) nc *= 2;
        res = grow_zero(res, rcap, nc);
        rcap = nc;
    }
}

seq_t a(idx_t n) {
    if (rlen == 0) {
        ensure_res(2);
        ensure_diag(3);
        res[0] = 1;
        res[1] = 1;
        rlen = 2;
        diag[1] = 1;   /* T(1,1) */
        s_cur = 2;
    }
    while (rlen <= n) {
        idx_t s = s_cur;           /* advance to antidiagonal s + 1 */
        ensure_diag(s + 2);
        ensure_res(s + 1);
        seq_t prev_old = diag[1];  /* T(1, s-1) */
        diag[1] = 1;               /* T(1, s) */
        for (idx_t k = 2; k <= s; k++) {
            idx_t x = s + 1 - k;
            seq_t old = 0;
            if (k < s) old = diag[k];
            seq_t xs = (seq_t)x;
            seq_t xp1 = xs + 1;
            seq_t inner = xp1 * diag[k - 1] - xs * prev_old;
            diag[k] = xp1 * inner;
            prev_old = old;
        }
        res[s] = diag[s];          /* T(s, 1) */
        s_cur = s + 1;
        rlen = s + 1;
    }
    return res[n];
}
