/*
 * A007787  Number of nonintersecting rook paths joining opposite corners of 5
 *          X n board.
 * https://oeis.org/A007787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   transfer-matrix method. Build the transfer matrix T over the
 *            boundary states of a 5-row strip, column by column. Its
 *            characteristic polynomial gives a linear recurrence a(n) =
 *            sum_{i=1..S} c_i*a(n-i), whose initial values are computed
 *            directly as entries of T^k. Check: a(1) = 1, a(2) = 16, a(3)
 *            =...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, rk_gs, rk_match_bwd, rk_match_fwd, rk_ss
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/rk_gs.h"
#include "../utils/rk_match_bwd.h"
#include "../utils/rk_match_fwd.h"
#include "../utils/rk_ss.h"

typedef unsigned __int128 u128_t;
#define MAXS 64

static int rk_nst = 0;
static int rk_code[MAXS];
static long long rk_T[MAXS][MAXS];
static int rk_sign[MAXS + 1];
static seq_t rk_cmag[MAXS + 1];
static seq_t rk_vec[MAXS], rk_nv[MAXS];
static seq_t *rk_u = NULL;
static idx_t rk_ulen = 0, rk_ucap = 0;
static long long rk_cur[4096], rk_nxt[4096], rk_out[1024];

static void rk_column(int rows) {
    int i, s;
    memset(rk_cur, 0, sizeof rk_cur);
    rk_cur[rows << 2] = 1;
    for (i = 0; i < 5; i++) {
        memset(rk_nxt, 0, sizeof rk_nxt);
        for (s = 0; s < 4096; s++) {
            long long c = rk_cur[s];
            int u, l, b, m;
            if (!c) continue;
            u = rk_gs(s, i); l = rk_gs(s, i + 1);
            b = rk_ss(rk_ss(s, i, 0), i + 1, 0);
            if (!u && !l) {
                rk_nxt[b] += c;
                if (i < 4) rk_nxt[rk_ss(rk_ss(b, i, 1), i + 1, 2)] += c;
            } else if (!u || !l) {
                int x = u ? u : l;
                rk_nxt[rk_ss(b, i, x)] += c;
                if (i < 4) rk_nxt[rk_ss(b, i + 1, x)] += c;
            } else if (u == 1 && l == 2) {
                /* closing a loop: forbidden */
            } else if (u == 2 && l == 1) {
                rk_nxt[b] += c;
            } else if (u == 1 && l == 1) {
                m = rk_match_fwd(s, i + 1);
                if (m >= 0) rk_nxt[rk_ss(b, m, 1)] += c;
            } else if (u == 2 && l == 2) {
                m = rk_match_bwd(s, i);
                if (m >= 0) rk_nxt[rk_ss(b, m, 2)] += c;
            } else if (u == 3) {
                m = (l == 1) ? rk_match_fwd(s, i + 1) : rk_match_bwd(s, i + 1);
                if (m >= 0) rk_nxt[rk_ss(b, m, 3)] += c;
            } else if (l == 3) {
                m = (u == 1) ? rk_match_fwd(s, i) : rk_match_bwd(s, i);
                if (m >= 0) rk_nxt[rk_ss(b, m, 3)] += c;
            }
        }
        memcpy(rk_cur, rk_nxt, sizeof rk_cur);
    }
    memset(rk_out, 0, sizeof rk_out);
    for (s = 0; s < 4096; s++)
        if (rk_cur[s] && rk_gs(s, 5) == 0) rk_out[s & 1023] += rk_cur[s];
}

static seq_t rk_from_mag(u128_t m) {
    seq_t r = 0;
    int s;
    for (s = 3; s >= 0; s--) {
        r = r * 65536;
        r = r * 65536;
        r += (idx_t)(uint32_t)(m >> (32 * s));
    }
    return r;
}

static void rk_init(void) {
    static int idx[1024];
    static u128_t M[MAXS][MAXS];
    u128_t p[MAXS + 2], q[MAXS + 2], t[MAXS + 2], v[MAXS], w[MAXS];
    int i, j, k, r, s, S, fi, mm;

    for (i = 0; i < 1024; i++) idx[i] = -1;
    rk_code[0] = 3; idx[3] = 0; rk_nst = 1;
    for (k = 0; k < rk_nst; k++) {
        rk_column(rk_code[k]);
        for (s = 0; s < 1024; s++) {
            if (!rk_out[s]) continue;
            if (idx[s] < 0) { idx[s] = rk_nst; rk_code[rk_nst++] = s; }
            rk_T[idx[s]][k] = rk_out[s];
        }
    }
    S = rk_nst;
    fi = idx[3 << 8];

    /* Berkowitz characteristic polynomial, exact modulo 2^128 */
    for (i = 0; i < S; i++)
        for (j = 0; j < S; j++) M[i][j] = (u128_t)rk_T[i][j];
    p[0] = 1;
    for (r = 0; r < S; r++) {
        t[0] = 1;
        t[1] = (u128_t)0 - M[r][r];
        for (k = 0; k < r; k++) v[k] = M[k][r];
        for (mm = 2; mm <= r + 1; mm++) {
            u128_t sum = 0;
            for (k = 0; k < r; k++) sum += M[r][k] * v[k];
            t[mm] = (u128_t)0 - sum;
            if (mm < r + 1) {
                for (i = 0; i < r; i++) {
                    u128_t z = 0;
                    for (k = 0; k < r; k++) z += M[i][k] * v[k];
                    w[i] = z;
                }
                for (i = 0; i < r; i++) v[i] = w[i];
            }
        }
        for (i = 0; i <= r + 1; i++) {
            u128_t z = 0;
            for (j = 0; j <= r && j <= i; j++) z += t[i - j] * p[j];
            q[i] = z;
        }
        for (i = 0; i <= r + 1; i++) p[i] = q[i];
    }
    for (i = 1; i <= S; i++) {
        u128_t x = (u128_t)0 - p[i];
        if (x == 0) { rk_sign[i] = 0; rk_cmag[i] = 0; }
        else if ((x >> 127) == 0) { rk_sign[i] = 1; rk_cmag[i] = rk_from_mag(x); }
        else { rk_sign[i] = -1; rk_cmag[i] = rk_from_mag(p[i]); }
    }

    rk_ucap = (idx_t)(2 * S + 16);
    rk_u = alloc_zero(rk_ucap);
    for (i = 0; i < S; i++) rk_vec[i] = 0;
    rk_vec[0] = 1;
    for (k = 0; k < S; k++) {
        rk_u[k] = (fi >= 0) ? rk_vec[fi] : (seq_t)0;
        for (j = 0; j < S; j++) rk_nv[j] = 0;
        for (j = 0; j < S; j++)
            for (i = 0; i < S; i++)
                if (rk_T[j][i]) rk_nv[j] += rk_vec[i] * (idx_t)rk_T[j][i];
        for (j = 0; j < S; j++) rk_vec[j] = rk_nv[j];
    }
    rk_ulen = (idx_t)S;
}

seq_t a(idx_t n) {
    int i, S;
    if (rk_u == NULL) rk_init();
    S = rk_nst;
    while (rk_ulen <= n) {
        idx_t k = rk_ulen;
        seq_t P = 0, N = 0;
        if (rk_ulen == rk_ucap) {
            idx_t nc = rk_ucap * 2;
            if (nc < n + 1) nc = n + 1;
            rk_u = grow_zero(rk_u, rk_ucap, nc);
            rk_ucap = nc;
        }
        for (i = 1; i <= S; i++) {
            if (rk_sign[i] > 0) P += rk_cmag[i] * rk_u[k - (idx_t)i];
            else if (rk_sign[i] < 0) N += rk_cmag[i] * rk_u[k - (idx_t)i];
        }
        rk_u[k] = P - N;
        rk_ulen++;
    }
    return rk_u[n];
}
