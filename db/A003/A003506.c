/*
 * A003506  Triangle of denominators in Leibniz's Harmonic Triangle a(n,k), n
 *          >= 1, 1 <= k <= n.
 * https://oeis.org/A003506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, nice, easy
 * Formula:   T(n,k) = n*binomial(n-1,k-1), 1 <= k <= n, read by rows
 * Range:     u8 n <= 31, u16 n <= 126, u32 n <= 479, u64 n <= 1981,
 *            u128 n <= 7260, big any n
 * Helpers:   a003506_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003506_g.h"

seq_t a(idx_t n) {
    idx_t row = 1, cum = 0;
    while (cum + row < n) { cum += row; row++; }
    idx_t k = n - cum;            /* 1 <= k <= row */
    idx_t m = row - 1;
    idx_t j = k - 1;
    if (m - j < j) j = m - j;
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < j; i++) {
        idx_t num = m - i, d = i + 1;
        idx_t rm = (idx_t)(long long)(r % (seq_t)d);
        idx_t g = a003506_g(rm, d);
        if (g == 0) g = d;
        r /= (seq_t)g;
        d /= g;
        r *= (seq_t)(num / d);
    }
    return (seq_t)row * r;
}
