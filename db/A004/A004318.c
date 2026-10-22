/*
 * A004318  Binomial coefficient C(2n,n-12).
 * https://oeis.org/A004318
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-12), computed multiplicatively as
 *            prod_{i=1..n-12} (n+12+i)/i
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 22, u64 n <= 36, u128 n <= 67,
 *            big any n
 * Helpers:   a004318_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004318_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 12;
    idx_t base = n + 12;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(base + i - 1, i - 1); next r = r * (base + i) / i, exactly */
        seq_t si = (seq_t)i;
        seq_t g = a004318_gcd(r, si);
        seq_t num = (seq_t)(base + i);
        r /= g;
        si /= g;
        num /= si;
        r *= num;
    }
    return r;
}
