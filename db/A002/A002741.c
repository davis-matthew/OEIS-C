/*
 * A002741  Logarithmic numbers: expansion of the e.g.f. -log(1-x) * e^(-x).
 * https://oeis.org/A002741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = (n-2)*a(n-1) + (n-1)*a(n-2) - (-1)^n, a(0) = 0, a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2 = 0;  /* a(i-2) */
    seq_t prev1 = 1;  /* a(i-1) */
    idx_t i;
    if (n == 0) return prev2;
    if (n == 1) return prev1;
    for (i = 2; i <= n; i++) {
        seq_t cur = ((seq_t)i - 2) * prev1 + ((seq_t)i - 1) * prev2;
        if (i & 1) cur += 1; else cur -= 1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
