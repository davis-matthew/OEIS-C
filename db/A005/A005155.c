/*
 * A005155  Number of degree sequences of n-node graphs.
 * https://oeis.org/A005155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (B_n(n) + C(n))/2, from the e.g.f. by Lagrange inversion,
 *            where C(n) = He_n(n+1) - n*He_{n-1}(n+1), He_0(y) = 1, He_1(y) =
 *            y, He_{m+1}(y) = y*He_m(y) - m*He_{m-1}(y); B_m(y) = m! [t^m]
 *            sqrt(1-t^2)*exp(y*t - t^2/2): B_0 = 1, B_{<0} = 0, B_{m+1} =
 *            y*B_m + m(m-3)*B_{m-1} - y*m(m-1)*B_{m-2} +...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 330 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

/* signed-magnitude accumulate: (m,s) += (v,vs) */
static void sacc(seq_t *m, bool *s, seq_t v, bool vs) {
    if (*s == vs) {
        *m += v;
    } else if (*m >= v) {
        *m -= v;
    } else {
        *m = v - *m;
        *s = vs;
    }
}

static seq_t big_a(idx_t n) {
    seq_t cm;
    bool cs = false;
    if (n == 0) {
        cm = 1;
    } else {
        idx_t y = n + 1;
        seq_t hp = 1, hc = (seq_t)y;
        bool hps = false, hcs = false;
        for (idx_t m = 1; m < n; m++) {
            seq_t nm = hc * y;
            bool ns = hcs;
            sacc(&nm, &ns, hp * m, !hps);
            hp = hc; hps = hcs;
            hc = nm; hcs = ns;
        }
        cm = hc; cs = hcs;
        sacc(&cm, &cs, hp * n, !hps);
    }

    seq_t b[4];
    bool bs[4];
    for (int i = 0; i < 4; i++) { b[i] = 0; bs[i] = false; }
    b[3] = 1;
    for (idx_t m = 0; m < n; m++) {
        seq_t nm = b[3] * n;
        bool ns = bs[3];
        if (m >= 1) {
            if (m != 3) {
                seq_t t = b[2] * m * (m > 3 ? m - 3 : 3 - m);
                sacc(&nm, &ns, t, (bool)(bs[2] ^ (m < 3)));
            }
            if (m >= 2) {
                seq_t t = b[1] * n * m * (m - 1);
                sacc(&nm, &ns, t, !bs[1]);
            }
            if (m >= 3) {
                seq_t t = b[0] * m * (m - 1) * (m - 2);
                sacc(&nm, &ns, t, bs[0]);
            }
        }
        b[0] = b[1]; bs[0] = bs[1];
        b[1] = b[2]; bs[1] = bs[2];
        b[2] = b[3]; bs[2] = bs[3];
        b[3] = nm; bs[3] = ns;
    }
    sacc(&b[3], &bs[3], cm, cs);
    return b[3] / 2;
}

static void w_add(uint64_t *r, const uint64_t *x, const uint64_t *y) {
    unsigned __int128 c = 0;
    for (int i = 0; i < 3; i++) {
        c += (unsigned __int128)x[i] + y[i];
        r[i] = (uint64_t)c;
        c >>= 64;
    }
}

static void w_mul(uint64_t *r, const uint64_t *x, uint64_t k) {
    unsigned __int128 c = 0;
    for (int i = 0; i < 3; i++) {
        c += (unsigned __int128)x[i] * k;
        r[i] = (uint64_t)c;
        c >>= 64;
    }
}

/* 192-bit two's complement arithmetic for fixed-width builds */
static void w_set(uint64_t *r, uint64_t v) { r[0] = v; r[1] = 0; r[2] = 0; }

static void w_sub(uint64_t *r, const uint64_t *x, const uint64_t *y) {
    unsigned __int128 c = 1;
    for (int i = 0; i < 3; i++) {
        c += (unsigned __int128)x[i] + (uint64_t)~y[i];
        r[i] = (uint64_t)c;
        c >>= 64;
    }
}

static seq_t fix_a(idx_t n) {
    uint64_t c[3], t[3], u[3];
    if (n == 0) {
        w_set(c, 1);
    } else {
        uint64_t y = n + 1;
        uint64_t hp[3], hc[3];
        w_set(hp, 1);
        w_set(hc, y);
        for (idx_t m = 1; m < n; m++) {
            w_mul(t, hc, y);
            w_mul(u, hp, m);
            w_sub(t, t, u);
            memcpy(hp, hc, sizeof hp);
            memcpy(hc, t, sizeof hc);
        }
        w_mul(u, hp, n);
        w_sub(c, hc, u);
    }

    uint64_t b[4][3], nm[3];
    memset(b, 0, sizeof b);
    b[3][0] = 1;
    for (idx_t m = 0; m < n; m++) {
        w_mul(nm, b[3], n);
        if (m >= 1) {
            if (m != 3) {
                w_mul(t, b[2], m);
                w_mul(t, t, m > 3 ? m - 3 : 3 - m);
                if (m > 3) w_add(nm, nm, t); else w_sub(nm, nm, t);
            }
            if (m >= 2) {
                w_mul(t, b[1], n);
                w_mul(t, t, m);
                w_mul(t, t, m - 1);
                w_sub(nm, nm, t);
            }
            if (m >= 3) {
                w_mul(t, b[0], m);
                w_mul(t, t, m - 1);
                w_mul(t, t, m - 2);
                w_add(nm, nm, t);
            }
        }
        memmove(b[0], b[1], 3 * sizeof b[0]);
        memcpy(b[3], nm, sizeof nm);
    }
    w_add(t, b[3], c);
    uint64_t s0 = (t[0] >> 1) | (t[1] << 63);
    uint64_t s1 = (t[1] >> 1) | (t[2] << 63);
    unsigned __int128 v = ((unsigned __int128)s1 << 64) | s0;
    return u128_to_seq(v);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return fix_a(n);
    return big_a(n);
}
