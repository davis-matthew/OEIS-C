/*
 * A007769  Number of chord diagrams with n chords; number of pairings on a
 *          necklace.
 * https://oeis.org/A007769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   2n*a(n) = Sum_{2n=pq} alpha(p,q)*phi(q), a(0) = 1, where
 *            alpha(p,q) = Sum_{k>=0} binomial(p,2k)*q^k*(2k-1)!! if q even, =
 *            q^(p/2)*(p-1)!! if q odd. For even q, alpha satisfies T(j) =
 *            T(j-1) + (j-1)*q*T(j-2), T(0) = T(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   alpha_s, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/alpha_s.h"

typedef unsigned __int128 u128_t;
#define PM ((uint64_t)2305843009213693951ULL) /* 2^61 - 1, prime */

static uint64_t phi_u(uint64_t n) {
    uint64_t r = n, m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

static uint64_t mulm(uint64_t x, uint64_t y) {
    return (uint64_t)((u128_t)x * y % PM);
}

static uint64_t powm(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= PM;
    while (e) {
        if (e & 1) r = mulm(r, b);
        b = mulm(b, b);
        e >>= 1;
    }
    return r;
}

/* alpha(p,q) mod 2^128 */
static u128_t alpha_w(idx_t p, idx_t q) {
    if (q % 2 == 1) {
        u128_t r = 1;
        for (idx_t i = 0; i < p / 2; i++) r *= (u128_t)q;
        for (idx_t i = 1; i < p; i += 2) r *= (u128_t)i;
        return r;
    }
    u128_t t0 = 1, t1 = 1;
    for (idx_t j = 2; j <= p; j++) {
        u128_t t2 = t1 + t0 * (u128_t)((j - 1) * q);
        t0 = t1;
        t1 = t2;
    }
    return t1;
}

/* alpha(p,q) mod PM */
static uint64_t alpha_p(idx_t p, idx_t q) {
    if (q % 2 == 1) {
        uint64_t r = 1, qm = q % PM;
        for (idx_t i = 0; i < p / 2; i++) r = mulm(r, qm);
        for (idx_t i = 1; i < p; i += 2) r = mulm(r, i % PM);
        return r;
    }
    uint64_t t0 = 1, t1 = 1;
    for (idx_t j = 2; j <= p; j++) {
        uint64_t t2 = (t1 + mulm(t0, ((j - 1) * q) % PM)) % PM;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t m = 2 * n;

    if (seq_is_big()) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                sum += alpha_s(d, e) * (seq_t)phi_u(e);
                if (d != e) sum += alpha_s(e, d) * (seq_t)phi_u(d);
            }
        }
        return sum / (seq_t)m;
    }

    /* Fixed width: sum mod 2^128 and mod PM, then recover a(n) mod 2^128 */
    u128_t sw = 0;
    uint64_t sp = 0;
    for (idx_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            idx_t e = m / d;
            sw += alpha_w(d, e) * (u128_t)phi_u(e);
            sp = (sp + mulm(alpha_p(d, e), phi_u(e) % PM)) % PM;
            if (d != e) {
                sw += alpha_w(e, d) * (u128_t)phi_u(d);
                sp = (sp + mulm(alpha_p(e, d), phi_u(d) % PM)) % PM;
            }
        }
    }
    unsigned s = 0;
    idx_t o = m;
    while ((o & 1) == 0) { o >>= 1; s++; }
    u128_t oi = (u128_t)o;
    for (int i = 0; i < 7; i++) oi *= (u128_t)2 - (u128_t)o * oi;
    u128_t mask = (((u128_t)1) << (128 - s)) - 1;
    u128_t r = ((sw >> s) * oi) & mask;       /* a(n) mod 2^(128-s) */
    uint64_t x = mulm(sp, powm(m % PM, PM - 2)); /* a(n) mod PM */
    uint64_t rm = (uint64_t)(r % PM);
    uint64_t t = mulm((x + PM - rm) % PM, powm(powm(2, 128 - s), PM - 2));
    u128_t res = r + (((u128_t)t) << (128 - s));
    return u128_to_seq(res);
}
