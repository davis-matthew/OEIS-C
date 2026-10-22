/*
 * A007827  Number of homeomorphically irreducible (or series-reduced) trees
 *          with n pendant nodes, or continua with n non-cut points, or
 *          leaves.
 * https://oeis.org/A007827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: 1+(1+x-B(x))*B(x), where B(x) = Sum b(k)*x^k is the g.f.
 *            for A000669, so a(0) = 1 and a(n) = b(n) + b(n-1) -
 *            Sum_{i=1..n-1} b(i)*b(n-i) for n >= 1. B satisfies 1 + 2*B(x) -
 *            x = Prod_{k>=1} (1-x^k)^(-b(k)) (Euler transform), b(1) = 1,
 *            b(0) = 0: with c(k) = Sum_{d|k} d*b(d) and e = Euler tran...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 25, u64 n <= 44, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

/* ---------- arbitrary-precision path (seq_t exact) ---------- */
static seq_t *Bb = 0, *Bc = 0, *Be = 0;
static idx_t Bsz = 0; /* arrays hold indices 0..Bsz-1 */

static void big_build(idx_t N) {
    idx_t sz = N + 1, n, k, m;
    if (Bsz) {
        release(Bb, Bsz);
        release(Bc, Bsz);
        release(Be, Bsz);
    }
    Bb = alloc_zero(sz);
    Bc = alloc_zero(sz);
    Be = alloc_zero(sz);
    Bsz = sz;
    Be[0] = 1;
    if (N < 1) return;
    Bb[1] = 1;
    Be[1] = 1;
    for (m = 1; m <= N; m++) Bc[m] += 1;
    for (n = 2; n <= N; n++) {
        seq_t s = Bc[n];
        for (k = 1; k < n; k++) s += Bc[k] * Be[n - k];
        s /= n;
        Bb[n] = s;
        Be[n] = s + s;
        for (m = n; m <= N; m += n) Bc[m] += s * n;
    }
}

/* ---------- fixed-width path: exact 256-bit intermediates ---------- */
typedef struct { uint64_t w[4]; } u256;

static u256 u_from(uint64_t x) {
    u256 r;
    r.w[0] = x; r.w[1] = 0; r.w[2] = 0; r.w[3] = 0;
    return r;
}

static u256 u_add(u256 a, u256 b) {
    u256 r;
    unsigned __int128 carry = 0;
    int i;
    for (i = 0; i < 4; i++) {
        unsigned __int128 t = (unsigned __int128)a.w[i] + b.w[i] + carry;
        r.w[i] = (uint64_t)t;
        carry = t >> 64;
    }
    return r;
}

static u256 u_sub(u256 a, u256 b) {
    u256 r;
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < 4; i++) {
        uint64_t x = a.w[i], y = b.w[i];
        uint64_t d = x - y - borrow;
        borrow = (x < y || (x == y && borrow)) ? 1 : 0;
        r.w[i] = d;
    }
    return r;
}

static u256 u_mul(u256 a, u256 b) {
    u256 r = u_from(0);
    int i, j;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        if (a.w[i] == 0) continue;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)a.w[i] * b.w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    return r;
}

static u256 u_div(u256 a, uint64_t d) {
    u256 q;
    unsigned __int128 rem = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | a.w[i];
        q.w[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    return q;
}

static u256 *Fb = 0, *Fc = 0, *Fe = 0;
static idx_t Fsz = 0;

static void fix_build(idx_t N) {
    idx_t sz = N + 1, n, k, m;
    free(Fb); free(Fc); free(Fe);
    Fb = (u256 *)calloc(sz, sizeof(u256));
    Fc = (u256 *)calloc(sz, sizeof(u256));
    Fe = (u256 *)calloc(sz, sizeof(u256));
    Fsz = sz;
    Fe[0] = u_from(1);
    if (N < 1) return;
    Fb[1] = u_from(1);
    Fe[1] = u_from(1);
    for (m = 1; m <= N; m++) Fc[m] = u_add(Fc[m], u_from(1));
    for (n = 2; n <= N; n++) {
        u256 s = Fc[n];
        for (k = 1; k < n; k++) s = u_add(s, u_mul(Fc[k], Fe[n - k]));
        s = u_div(s, (uint64_t)n);
        Fb[n] = s;
        Fe[n] = u_add(s, s);
        {
            u256 t = u_mul(s, u_from((uint64_t)n));
            for (m = n; m <= N; m += n) Fc[m] = u_add(Fc[m], t);
        }
    }
}

seq_t a(idx_t n) {
    idx_t i;
    if (n == 0) return 1;
    if (seq_is_big()) {
        seq_t s = 0, r;
        if (n + 1 > Bsz) {
            idx_t N = n;
            if (Bsz && 2 * (Bsz - 1) > N) N = 2 * (Bsz - 1);
            big_build(N);
        }
        for (i = 1; i < n; i++) s += Bb[i] * Bb[n - i];
        r = Bb[n] + Bb[n - 1] - s;
        return r;
    } else {
        u256 s = u_from(0), r;
        /* beyond this index no term fits in 128 bits (and 256-bit intermediates overflow) */
        if (n > 400) return 0;
        if (n + 1 > Fsz) {
            idx_t N = n;
            if (Fsz && 2 * (Fsz - 1) > N) N = 2 * (Fsz - 1);
            if (N > 400) N = 400;
            fix_build(N);
        }
        for (i = 1; i < n; i++) s = u_add(s, u_mul(Fb[i], Fb[n - i]));
        r = u_sub(u_add(Fb[n], Fb[n - 1]), s);
        return u128_to_seq(((unsigned __int128)r.w[1] << 64) | r.w[0]);
    }
}
