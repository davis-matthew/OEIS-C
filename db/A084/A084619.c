/*
 * A084619  Duplicate of A023393.
 * https://oeis.org/A084619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^2 - 5n + 14 for n ≥ 8 a(1)=0, a(2)=1, a(3)=2, a(4)=7,
 *            a(5)=11, a(6)=19, a(7)=27 (the first 7 terms are taken from the
 *            sequence definition)
 * Range:     i8 n <= 13, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 7) {
        static const seq_t init[] = {0, 1, 2, 7, 11, 19, 27};
        return init[n - 1];
    }
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)5 * nn + (seq_t)14;
}
