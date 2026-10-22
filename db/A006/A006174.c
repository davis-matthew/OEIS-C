/*
 * A006174  Witt vector *3!.
 * https://oeis.org/A006174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3n)!/(n!)^3 - Sum_{d|n, d<n} d*a(d)^(n/d)) / n, i.e.
 *            the Witt vector whose ghost components are A006480(n) =
 *            (3n)!/(n!)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   b_pow, grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/b_pow.h"

/* ---------- fixed-width builds: exact 256-bit internal arithmetic ---------- */
typedef struct { uint64_t v[4]; } u256;

static u256 u_small(uint64_t x)
{
    u256 r;
    r.v[0] = x; r.v[1] = 0; r.v[2] = 0; r.v[3] = 0;
    return r;
}

static u256 u_sub(u256 a, u256 b)
{
    u256 r;
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < 4; i++) {
        uint64_t x = a.v[i], y = b.v[i];
        r.v[i] = x - y - borrow;
        borrow = (x < y || (x == y && borrow)) ? 1 : 0;
    }
    return r;
}

static u256 u_mul(u256 a, u256 b)
{
    u256 r;
    int i, j;
    for (i = 0; i < 4; i++) r.v[i] = 0;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)a.v[i] * b.v[j] + r.v[i + j] + carry;
            r.v[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    return r;
}

static u256 u_div(u256 a, uint64_t d)
{
    u256 q;
    unsigned __int128 rem = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | a.v[i];
        q.v[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    return q;
}

static u256 u_pow(u256 b, idx_t e)
{
    u256 r = u_small(1);
    while (e) {
        if (e & 1) r = u_mul(r, b);
        e >>= 1;
        if (e) b = u_mul(b, b);
    }
    return r;
}

static u256 *UC = NULL;
static idx_t ucap = 0;
static u256 UW;

static void u_extend(idx_t n)
{
    idx_t m, d;
    if (n <= ucap) return;
    UC = (u256 *)realloc(UC, (size_t)(n + 1) * sizeof(u256));
    if (ucap == 0) UW = u_small(1);
    for (m = ucap + 1; m <= n; m++) {
        u256 s;
        UW = u_mul(UW, u_small(3 * m));
        UW = u_mul(UW, u_small(3 * m - 1));
        UW = u_mul(UW, u_small(3 * m - 2));
        UW = u_div(u_div(u_div(UW, m), m), m);
        s = UW;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d < m) s = u_sub(s, u_mul(u_small(d), u_pow(UC[d], e)));
                if (e != d && e < m) s = u_sub(s, u_mul(u_small(e), u_pow(UC[e], d)));
            }
        }
        UC[m] = u_div(s, m);
    }
    ucap = n;
}

/* ---------- arbitrary-precision build ---------- */
static seq_t *BC = NULL;
static idx_t bcap = 0, blen = 0;
static seq_t BW;

static void b_extend(idx_t n)
{
    idx_t m, d;
    if (n <= bcap) return;
    BC = grow_zero(BC, blen, n + 1);
    blen = n + 1;
    if (bcap == 0) BW = 1;
    for (m = bcap + 1; m <= n; m++) {
        seq_t s;
        BW = BW * (seq_t)(3 * m);
        BW = BW * (seq_t)(3 * m - 1);
        BW = BW * (seq_t)(3 * m - 2);
        BW = BW / (seq_t)m;
        BW = BW / (seq_t)m;
        BW = BW / (seq_t)m;
        s = BW;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d < m) s -= (seq_t)d * b_pow(BC[d], e);
                if (e != d && e < m) s -= (seq_t)e * b_pow(BC[e], d);
            }
        }
        BC[m] = s / (seq_t)m;
    }
    bcap = n;
}

seq_t a(idx_t n)
{
    if (seq_is_big()) {
        b_extend(n);
        return BC[n];
    } else {
        unsigned __int128 x;
        u_extend(n);
        x = ((unsigned __int128)UC[n].v[1] << 64) | UC[n].v[0];
        return u128_to_seq(x);
    }
}
