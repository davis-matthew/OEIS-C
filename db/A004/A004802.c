/*
 * A004802  Numbers that are the sum of 2 nonzero 10th powers.
 * https://oeis.org/A004802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th smallest distinct value of i^10 + j^10 with 1 <= i
 *            <= j; computed by a k-way merge search over rows i (row i walks
 *            j = i, i+1, ...)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 43, u64 n <= 3516,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_arr.h"

static seq_t *pw = NULL;
static idx_t pwlen = 0, pwcap = 0, pw_ovf = 0;

/* b^10, false if it does not fit in seq_t */
static bool getpow(idx_t b, seq_t *out) {
    if (pw_ovf && b >= pw_ovf) return false;
    while (pwlen <= b) {
        if (pwlen == pwcap) {
            idx_t nc = pwcap ? pwcap * 2 : 64;
            pw = grow_arr(pw, pwcap, nc);
            pwcap = nc;
        }
        seq_t x = 1;
        bool ok = true;
        if (pwlen == 0) {
            x = 0;
        } else {
            seq_t bb = (seq_t)pwlen;
            int k;
            for (k = 0; k < 10; k++) {
                seq_t y = x * bb;
                if (y / bb != x) { ok = false; break; }
                x = y;
            }
        }
        if (!ok) { pw_ovf = pwlen; return false; }
        pw[pwlen++] = x;
    }
    *out = pw[b];
    return true;
}

static bool pairval(idx_t i, idx_t j, seq_t *out) {
    seq_t x, y;
    if (!getpow(i, &x) || !getpow(j, &y)) return false;
    seq_t s = x + y;
    if (s < x) return false;
    *out = s;
    return true;
}

seq_t a(idx_t n) {
    static seq_t *T = NULL;
    static idx_t tcnt = 0, tcap = 0;
    static idx_t *rj = NULL;
    static seq_t *rv = NULL;
    static char *rdead = NULL;
    static idx_t R = 0, rcap = 0;
    static bool done = false;

    while (tcnt < n && !done) {
        idx_t best = 0;
        seq_t bv = 0;
        bool have = false;
        idx_t i;
        for (i = 1; i <= R; i++) {
            if (!rdead[i] && (!have || rv[i] < bv)) {
                bv = rv[i];
                best = i;
                have = true;
            }
        }
        seq_t cv;
        if (pairval(R + 1, R + 1, &cv) && (!have || cv < bv)) {
            if (R + 2 > rcap) {
                idx_t nc = rcap ? rcap * 2 : 64;
                rv = grow_arr(rv, rcap, nc);
                rj = (idx_t *)realloc(rj, nc * sizeof(idx_t));
                rdead = (char *)realloc(rdead, nc * sizeof(char));
                rcap = nc;
            }
            R++;
            rj[R] = R;
            rv[R] = cv;
            rdead[R] = 0;
            best = R;
            bv = cv;
            have = true;
        }
        if (!have) { done = true; break; }

        if (tcnt == 0 || bv != T[tcnt - 1]) {
            if (tcnt == tcap) {
                idx_t nc = tcap ? tcap * 2 : 1024;
                T = grow_arr(T, tcap, nc);
                tcap = nc;
            }
            T[tcnt++] = bv;
        }

        rj[best]++;
        seq_t nv;
        if (pairval(best, rj[best], &nv)) rv[best] = nv;
        else rdead[best] = 1;
    }

    if (n == 0 || n > tcnt) return 0;
    return T[n - 1];
}
