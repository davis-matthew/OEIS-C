/*
 * A003063  a(n) = 3^(n-1) - 2^n.
 * https://oeis.org/A003063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) + 2^(n-1), a(1) = -1 (equivalently a(n) =
 *            3^(n-1) - 2^n)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = -1;
    seq_t p = 1; /* 2^(k-1) */
    idx_t k;
    for (k = 2; k <= n; k++) {
        p = p * 2;
        v = v * 3 + p;
    }
    return v;
}
