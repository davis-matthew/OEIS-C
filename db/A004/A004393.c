/*
 * A004393  Binomial coefficient C(8n,n-12).
 * https://oeis.org/A004393
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-12), computed multiplicatively as
 *            prod_{i=1..n-12} (7*n+12+i)/i, with gcd reduction so
 *            intermediates never exceed the result
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 17, u64 n <= 24, u128 n <= 29,
 *            big any n
 * Helpers:   a004393_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004393_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 12;
    idx_t base = 7 * n + 12;   /* m - k where m = 8n */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t si = (seq_t)i;
        seq_t g = a004393_gcd(r, si);
        seq_t num = (seq_t)(base + i);
        r /= g;
        r *= num / (si / g);
    }
    return r;
}
