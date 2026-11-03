/*
 * A005419  Number of nonequivalent dissections of a polygon into n heptagons
 *          by nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A005419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (12*G(z) - 5*G(z)^2 + 21*G(z^2) + 14*z*G(z^2)^3 +
 *            12*z*G(z^7)) / 28, where G(z) = 1 + z*G(z)^6 (A002295). Uses
 *            [z^m] G(z)^r = r/(6m+r) * binomial(6m+r, m), so a(n) = (12*T1(n)
 *            - 5*T2(n) + 21*[n even]*T1(n/2) + 14*[n odd]*T3((n-1)/2) + 12*[n
 *            == 1 mod 7]*T1((n-1)/7)) / 28, with Tr(m) = r*bino...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 850 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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

static seq_t TS(idx_t r, idx_t m) {
    return binomial(6 * m + r, m) * (seq_t)r / (seq_t)(6 * m + r);
}

typedef unsigned __int128 u128;

static u128 binU(idx_t N, idx_t k) {
    u128 r = 1;
    idx_t i;
    for (i = 0; i < k; i++) r = r * (u128)(N - i) / (u128)(i + 1);
    return r;
}

static u128 TU(idx_t r, idx_t m) {
    return binU(6 * m + r, m) * (u128)r / (u128)(6 * m + r);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        u128 s = 12 * TU(1, n);
        if (n % 2 == 0) s += 21 * TU(1, n / 2);
        else s += 14 * TU(3, (n - 1) / 2);
        if ((n - 1) % 7 == 0) s += 12 * TU(1, (n - 1) / 7);
        s -= 5 * TU(2, n);
        return u128_to_seq(s / 28);
    } else {
        seq_t s = 12 * TS(1, n);
        if (n % 2 == 0) s += 21 * TS(1, n / 2);
        else s += 14 * TS(3, (n - 1) / 2);
        if ((n - 1) % 7 == 0) s += 12 * TS(1, (n - 1) / 7);
        s -= 5 * TS(2, n);
        return s / 28;
    }
}
