/*
 * A203423  a(n) = w(n+1)/(2*w(n)), where w=A203422.
 * https://oeis.org/A203423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (n+1)*(n+2)^n / 2 (= (-1)^n*A053506(n+2)/2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 338 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A203423_pow(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e > 0) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t m = (seq_t)n + 2;
    seq_t res;
    if ((n & 1) == 1) {
        /* n+1 even */
        res = ((seq_t)n + 1) / 2 * A203423_pow(m, n);
    } else {
        /* n+2 even */
        res = (m / 2) * A203423_pow(m, n - 1) * ((seq_t)n + 1);
    }
    if (n & 1) res = -res;
    return res;
}
