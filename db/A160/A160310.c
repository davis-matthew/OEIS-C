/*
 * A160310  Numerator of Hermite(n, 12/31).
 * https://oeis.org/A160310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 24*a(n+1) - 1922*(n+1)*a(n), a(0)=1, a(1)=24
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 24;
    seq_t a0 = 1;
    seq_t a1 = 24;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = ((seq_t)1922) * ((seq_t)(i - 1));
        seq_t new_val = ((seq_t)24) * a1 - mult * a0;
        a0 = a1;
        a1 = new_val;
    }
    return a1;
}
