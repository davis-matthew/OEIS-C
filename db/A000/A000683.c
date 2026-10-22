/*
 * A000683  Number of colorings of labeled graphs on n nodes using exactly 2
 *          colors, divided by 4.
 * https://oeis.org/A000683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/4) * Sum_{i=1..n-1} binomial(n,i) * 2^(i*(n-i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   a683_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a683_width.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    int w = a683_width();
    seq_t result = 0;
    seq_t binom = 1; /* binomial(n, i) */
    for (idx_t i = 1; i < n; i++) {
        binom = binom * (seq_t)(n - i + 1) / (seq_t)i;
        idx_t e = i * (n - i) - 2; /* >= 0 for n >= 3 */
        if (w != 0 && e >= (idx_t)w) continue;
        result += (seq_t)(binom << e);
    }
    return result;
}
