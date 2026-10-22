/*
 * A005129  Theta series of {E_6}* lattice.
 * https://oeis.org/A005129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(0) = 1; for n >= 1, a(n) = 9 * Sum_{d|n} (chi(n/d) - chi(d)) *
 *            d^2, where chi(m) = Kronecker symbol (m/3), i.e. 1 if m == 1
 *            (mod 3), -1 if m == 2 (mod 3), 0 if 3 | m. (Theta_{E6*} =
 *            combination of the weight-3 Eisenstein series for chi_{-3}.)
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int chi3(idx_t m) {
    idx_t r = m % 3;
    if (r == 1) return 1;
    if (r == 2) return -1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int w = chi3(n / d) - chi3(d);
        if (w == 0) continue;
        seq_t sq = (seq_t)d * (seq_t)d;
        if (w > 0) {
            for (int k = 0; k < w; k++) pos += sq;
        } else {
            for (int k = 0; k < -w; k++) neg += sq;
        }
    }
    return (seq_t)9 * (pos - neg);
}
