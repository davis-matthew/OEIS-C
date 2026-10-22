/*
 * A005008  a(n) = n! - n^2.
 * https://oeis.org/A005008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - n^2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t factorial = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    seq_t n_seq = (seq_t)n;
    return factorial - n_seq * n_seq;
}
