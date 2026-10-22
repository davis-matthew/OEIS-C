/*
 * A005195  Number of forests with n unlabeled nodes.
 * https://oeis.org/A005195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (1/n)*Sum_{k=1..n} b(k)*a(n-k), a(0) = 1, where b(k) =
 *            Sum_{d|k} d*A000055(d); A000055(n) = A000081(n) -
 *            (Sum_{i=1..n-1} A000081(i)*A000081(n-i) - [n
 *            even]*A000081(n/2))/2; A000081(n+1) = (1/n)*Sum_{k=1..n}
 *            s(k)*A000081(n-k+1), A000081(1) = 1, s(k) = Sum_{d|k}
 *            d*A000081(d)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 27, u64 n <= 49, u128 n <= 92,
 *            big any n
 * Helpers:   seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

/* ---- fixed-width path: exact 256-bit arithmetic, then truncate ---- */
typedef struct { uint32_t w[8]; } U256;

static U256 u_from(uint64_t v) {
    U256 r;
    int i;
    for (i = 0; i < 8; i++) r.w[i] = 0;
    r.w[0] = (uint32_t)v;
    r.w[1] = (uint32_t)(v >> 32);
    return r;
}

static U256 u_add(U256 a, U256 b) {
    U256 r;
    uint64_t c = 0;
    int i;
    for (i = 0; i < 8; i++) {
        c += (uint64_t)a.w[i] + b.w[i];
        r.w[i] = (uint32_t)c;
        c >>= 32;
    }
    return r;
}

static U256 u_sub(U256 a, U256 b) {
    U256 r;
    uint64_t br = 0;
    int i;
    for (i = 0; i < 8; i++) {
        uint64_t x = (uint64_t)a.w[i] - (uint64_t)b.w[i] - br;
        r.w[i] = (uint32_t)x;
        br = (x >> 32) ? 1 : 0;
    }
    return r;
}

static U256 u_mul(U256 a, U256 b) {
    U256 r = u_from(0);
    int i, j;
    for (i = 0; i < 8; i++) {
        uint64_t carry = 0;
        if (a.w[i] == 0) continue;
        for (j = 0; i + j < 8; j++) {
            uint64_t cur = (uint64_t)a.w[i] * b.w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    return r;
}

static U256 u_div(U256 a, uint32_t d) {
    U256 r;
    uint64_t rem = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a.w[i];
        r.w[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    return r;
}

static seq_t u_to_seq(U256 a) {
    unsigned __int128 v = 0;
    int i;
    for (i = 3; i >= 0; i--) v = (v << 32) | a.w[i];
    return u128_to_seq(v);
}

#define FMAX 302
static U256 fr[FMAX], fs[FMAX], ft[FMAX], fb[FMAX], fa[FMAX];
static idx_t flen = 0;

static void f_extend(idx_t N) {
    idx_t m, k, d;
    for (m = flen; m <= N; m++) {
        if (m == 0) {
            fr[0] = u_from(0); fs[0] = u_from(0); ft[0] = u_from(0);
            fb[0] = u_from(0); fa[0] = u_from(1);
            continue;
        }
        if (m == 1) {
            fr[1] = u_from(1);
        } else {
            U256 sum = u_from(0);
            for (k = 1; k <= m - 1; k++) sum = u_add(sum, u_mul(fs[k], fr[m - k]));
            fr[m] = u_div(sum, (uint32_t)(m - 1));
        }
        {
            U256 s = u_from(0), b, c = u_from(0), sum = u_from(0);
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s = u_add(s, u_mul(u_from(d), fr[d]));
                    if (e != d) s = u_add(s, u_mul(u_from(e), fr[e]));
                }
            }
            fs[m] = s;
            for (k = 1; k + 1 <= m; k++) c = u_add(c, u_mul(fr[k], fr[m - k]));
            if (m % 2 == 0) c = u_sub(c, fr[m / 2]);
            ft[m] = u_sub(fr[m], u_div(c, 2));
            b = u_from(0);
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    b = u_add(b, u_mul(u_from(d), ft[d]));
                    if (e != d) b = u_add(b, u_mul(u_from(e), ft[e]));
                }
            }
            fb[m] = b;
            for (k = 1; k <= m; k++) sum = u_add(sum, u_mul(fb[k], fa[m - k]));
            fa[m] = u_div(sum, (uint32_t)m);
        }
    }
    if (N + 1 > flen) flen = N + 1;
}

/* ---- arbitrary-precision path ---- */
#define BMAX 10050
static seq_t br_[BMAX], bs_[BMAX], bt_[BMAX], bb_[BMAX], ba_[BMAX];
static idx_t blen = 0;

static void b_extend(idx_t N) {
    idx_t m, k, d;
    for (m = blen; m <= N; m++) {
        if (m == 0) {
            br_[0] = 0; bs_[0] = 0; bt_[0] = 0; bb_[0] = 0; ba_[0] = 1;
            continue;
        }
        if (m == 1) {
            br_[1] = 1;
        } else {
            seq_t sum = 0;
            for (k = 1; k <= m - 1; k++) sum += bs_[k] * br_[m - k];
            br_[m] = sum / (seq_t)(m - 1);
        }
        {
            seq_t s = 0, b = 0, c = 0, sum = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s += (seq_t)d * br_[d];
                    if (e != d) s += (seq_t)e * br_[e];
                }
            }
            bs_[m] = s;
            for (k = 1; k + 1 <= m; k++) c += br_[k] * br_[m - k];
            if (m % 2 == 0) c -= br_[m / 2];
            bt_[m] = br_[m] - c / 2;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    b += (seq_t)d * bt_[d];
                    if (e != d) b += (seq_t)e * bt_[e];
                }
            }
            bb_[m] = b;
            for (k = 1; k <= m; k++) sum += bb_[k] * ba_[m - k];
            ba_[m] = sum / (seq_t)m;
        }
    }
    if (N + 1 > blen) blen = N + 1;
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        if (n >= BMAX) return 0;
        if (n >= blen) b_extend(n);
        return ba_[n];
    }
    /* beyond this index no term fits in 128 bits */
    if (n >= FMAX) return 0;
    if (n >= flen) f_extend(n);
    return u_to_seq(fa[n]);
}
