/*
 * A006965  Number of directed trees with n nodes.
 * https://oeis.org/A006965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n) - (3*Sum_{i=1..n-1} r(i)*r(n-i) - [n
 *            even]*r(n/2))/2, where r = rooted directed trees (edges ->, <-,
 *            <->): r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m} s(k)*r(m-k+1),
 *            s(k) = Sum_{d|k} 3*d*r(d) (r(x) = x * Euler transform of 3*r)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

typedef struct { uint64_t l[4]; } w256;

static w256 w_from(uint64_t v) {
    w256 r; r.l[0] = v; r.l[1] = 0; r.l[2] = 0; r.l[3] = 0; return r;
}
static w256 w_add(w256 a, w256 b) {
    w256 r; unsigned __int128 c = 0; int i;
    for (i = 0; i < 4; i++) {
        c += (unsigned __int128)a.l[i] + b.l[i];
        r.l[i] = (uint64_t)c; c >>= 64;
    }
    return r;
}
static w256 w_sub(w256 a, w256 b) {
    w256 r; unsigned __int128 c = 1; int i;
    for (i = 0; i < 4; i++) {
        c += (unsigned __int128)a.l[i] + (uint64_t)~b.l[i];
        r.l[i] = (uint64_t)c; c >>= 64;
    }
    return r;
}
static w256 w_mul(w256 a, w256 b) {
    w256 r = w_from(0); int i, j;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)a.l[i] * b.l[j] + r.l[i + j] + carry;
            r.l[i + j] = (uint64_t)t; carry = t >> 64;
        }
    }
    return r;
}
static w256 w_div(w256 a, uint64_t d) {
    w256 q; unsigned __int128 rem = 0; int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | a.l[i];
        q.l[i] = (uint64_t)(cur / d); rem = cur % d;
    }
    return q;
}

#define WMAX 60
static w256 WR[WMAX + 1], WS[WMAX + 1];
static int w_done = 0;

static void w_init(void) {
    idx_t m, k, d;
    WR[1] = w_from(1); WS[1] = w_from(3);
    for (m = 1; m < WMAX; m++) {
        w256 sum = w_from(0);
        for (k = 1; k <= m; k++) sum = w_add(sum, w_mul(WS[k], WR[m - k + 1]));
        WR[m + 1] = w_div(sum, m);
        k = m + 1;
        sum = w_from(0);
        for (d = 1; d * d <= k; d++) if (k % d == 0) {
            sum = w_add(sum, w_mul(w_from(d), WR[d]));
            if (k / d != d) sum = w_add(sum, w_mul(w_from(k / d), WR[k / d]));
        }
        WS[k] = w_mul(sum, w_from(3));
    }
    w_done = 1;
}

static seq_t a_fixed(idx_t n) {
    w256 t; idx_t i; seq_t v = 0; int j, s;
    if (n > WMAX) return 0;
    if (!w_done) w_init();
    t = w_from(0);
    for (i = 1; i < n; i++) t = w_add(t, w_mul(WR[i], WR[n - i]));
    t = w_mul(t, w_from(3));
    if (n % 2 == 0) t = w_sub(t, WR[n / 2]);
    t = w_div(t, 2);
    t = w_sub(WR[n], t);
    for (j = 3; j >= 0; j--) {
        for (s = 0; s < 4; s++) v <<= 16;
        v += (seq_t)(uint64_t)t.l[j];
    }
    return v;
}

static seq_t *BR = 0, *BS = 0;
static idx_t b_have = 0, b_cap = 0;

static seq_t a_big(idx_t n) {
    idx_t i, m, k, d;
    seq_t t;
    if (b_cap < n + 1) {
        idx_t nc = b_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (b_cap == 0) { BR = alloc_zero(nc); BS = alloc_zero(nc); }
        else { BR = grow_zero(BR, b_cap, nc); BS = grow_zero(BS, b_cap, nc); }
        b_cap = nc;
    }
    if (b_have == 0) { BR[1] = 1; BS[1] = 3; b_have = 1; }
    while (b_have < n) {
        seq_t sum = 0;
        m = b_have;
        for (k = 1; k <= m; k++) sum += BS[k] * BR[m - k + 1];
        BR[m + 1] = sum / (seq_t)m;
        k = m + 1;
        sum = 0;
        for (d = 1; d * d <= k; d++) if (k % d == 0) {
            sum += (seq_t)d * BR[d];
            if (k / d != d) sum += (seq_t)(k / d) * BR[k / d];
        }
        BS[k] = sum * 3;
        b_have = k;
    }
    t = 0;
    for (i = 1; i < n; i++) t += BR[i] * BR[n - i];
    t *= 3;
    if (n % 2 == 0) t -= BR[n / 2];
    t /= 2;
    return BR[n] - t;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return a_big(n);
    return a_fixed(n);
}
