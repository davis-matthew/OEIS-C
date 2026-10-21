/*
 * A083596  Duplicate of A082541.
 * https://oeis.org/A083596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1; a(n)=7*3^(n-1) for n>=1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)7;
    for (idx_t i = 1; i < n; ++i) {
        res *= (seq_t)3;
    }
    return res;
}
