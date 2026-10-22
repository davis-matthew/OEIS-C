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
 * Helpers:   TS, binS, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/TS.h"
#include "../utils/binS.h"

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
