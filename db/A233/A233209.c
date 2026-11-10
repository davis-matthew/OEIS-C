/*
 * A233209  Records in A233208.
 * https://oeis.org/A233209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/|q*e - round(q*e)|) with q = A007677(n), the n-th
 *            * convergent denominator of e (q = 1, 3, 4, 7, 32, ...). The
 *            value is computed * in 128-bit-mantissa floating point (PARI's
 *            default precision), which is how * the OEIS terms were made: e
 *            is rounded to 128 bits, then q*e is rounded t...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* i-th partial quotient of e */
static idx_t A233209_cf(idx_t i)
{
    if (i == 0) return 2;
    if (i % 3 == 2) return 2 * (i + 1) / 3;
    return 1;
}

/* exact floor(q * [0; c(s), c(s+1), ...]) via bracketing convergents */
static seq_t A233209_floor_frac(seq_t q, idx_t s)
{
    seq_t pm2 = 1, pm1 = 0;
    seq_t gm2 = 0, gm1 = 1;
    seq_t prev = 0;
    for (idx_t j = 0;; ++j) {
        seq_t c = (seq_t)A233209_cf(s + j);
        seq_t p = c * pm1 + pm2;
        seq_t g = c * gm1 + gm2;
        pm2 = pm1; pm1 = p;
        gm2 = gm1; gm1 = g;
        seq_t f = (q / g) * p + ((q % g) * p) / g;
        if (j > 0 && f == prev) return f;
        prev = f;
    }
}

static seq_t A233209_exact(idx_t n)
{
    seq_t qkm1 = 0, qk = 1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t nq = (seq_t)A233209_cf(j) * qk + qkm1;
        qkm1 = qk;
        qk = nq;
    }
    seq_t qnext = (seq_t)A233209_cf(n + 1) * qk + qkm1;
    return qnext + A233209_floor_frac(qk, n + 2);
}

/* e rounded to nearest with a 128-bit mantissa, as an integer scaled by 2^126 */
static unsigned __int128 A233209_e128(void)
{
    uint64_t S[4] = {0, 0, 0, 0};
    uint64_t T[4] = {0, 0, 0, 1};          /* 2^192 */
    for (int k = 0; k <= 60; ++k) {
        if (k > 0) {
            unsigned __int128 r = 0;
            for (int i = 3; i >= 0; --i) {
                unsigned __int128 cur = (r << 64) | T[i];
                T[i] = (uint64_t)(cur / (unsigned)k);
                r = cur % (unsigned)k;
            }
        }
        unsigned __int128 carry = 0;
        for (int i = 0; i < 4; ++i) {
            unsigned __int128 sum = (unsigned __int128)S[i] + T[i] + carry;
            S[i] = (uint64_t)sum;
            carry = sum >> 64;
        }
    }
    /* S = e * 2^192; take bits 66.. with rounding */
    uint64_t lo = (S[1] >> 2) | (S[2] << 62);
    uint64_t hi = (S[2] >> 2) | (S[3] << 62);
    unsigned __int128 E = ((unsigned __int128)hi << 64) | lo;
    if ((S[1] >> 1) & 1) E += 1;
    return E;
}

static seq_t A233209_from_u128(unsigned __int128 v)
{
    uint64_t hi = (uint64_t)(v >> 64), lo = (uint64_t)v;
    seq_t r = (seq_t)hi;
    if (hi) { r <<= 32; r <<= 32; }
    r += (seq_t)lo;
    return r;
}

seq_t a(idx_t n)
{
    /* q_n in 128-bit; fall back to exact method if it exceeds 64 bits */
    unsigned __int128 qkm1 = 0, qk = 1;
    const unsigned __int128 LIM = (unsigned __int128)1 << 64;
    for (idx_t j = 1; j <= n; ++j) {
        unsigned __int128 nq = (unsigned __int128)A233209_cf(j) * qk + qkm1;
        qkm1 = qk;
        qk = nq;
        if (qk >= LIM) return A233209_exact(n);
    }
    uint64_t q = (uint64_t)qk;

    static bool init = false;
    static unsigned __int128 E;
    if (!init) { E = A233209_e128(); init = true; }

    /* P = q * E exactly (192 bits, scaled by 2^126) */
    unsigned __int128 x = (unsigned __int128)q * (uint64_t)E;
    unsigned __int128 y = (unsigned __int128)q * (uint64_t)(E >> 64);
    uint64_t p0 = (uint64_t)x;
    unsigned __int128 c = (x >> 64) + (uint64_t)y;
    uint64_t p1 = (uint64_t)c;
    uint64_t p2 = (uint64_t)((c >> 64) + (y >> 64));

    /* round P to 128 significant bits (round half up) */
    int bl;
    if (p2) { bl = 128; uint64_t t = p2; while (t) { ++bl; t >>= 1; } }
    else if (p1) { bl = 64; uint64_t t = p1; while (t) { ++bl; t >>= 1; } }
    else { bl = 0; uint64_t t = p0; while (t) { ++bl; t >>= 1; } }
    if (bl > 128) {
        int s = bl - 128;                   /* 1..64 */
        uint64_t rb = (p0 >> (s - 1)) & 1;
        if (s == 64) p0 = 0;
        else p0 &= ~(((uint64_t)1 << s) - 1);
        if (rb) {
            unsigned __int128 add = (s == 64) ? 0 : ((uint64_t)1 << s);
            unsigned __int128 t0 = (unsigned __int128)p0 + (uint64_t)add;
            unsigned __int128 t1 = (unsigned __int128)p1 + (t0 >> 64) + (s == 64 ? 1 : 0);
            p0 = (uint64_t)t0;
            p1 = (uint64_t)t1;
            p2 = p2 + (uint64_t)(t1 >> 64);
        }
    }

    /* fractional part of F / 2^126, distance to nearest integer */
    const unsigned __int128 ONE = (unsigned __int128)1 << 126;
    unsigned __int128 frac = ((unsigned __int128)(p1 & ((((uint64_t)1) << 62) - 1)) << 64) | p0;
    unsigned __int128 D = (frac >= (ONE >> 1)) ? ONE - frac : frac;
    if (D == 0) return A233209_exact(n);
    return A233209_from_u128(ONE / D);
}
