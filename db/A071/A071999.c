/*
 * A071999  Determinant of n X n matrix whose element A(i,j) is 1 if i=j, i if
 *          n=i+j and 0 otherwise.
 * https://oeis.org/A071999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = product_{i=1..⌊(n-1)/2⌋} (1 - i*(n-i)) (a(0)=a(1)=a(2)=1)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) {
        return (seq_t)1;
    }
    seq_t result = (seq_t)1;
    /* floor((n-1)/2) */
    idx_t limit = (n - 1) >> 1;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t term = (seq_t)i * (seq_t)(n - i);
        term = (seq_t)1 - term;
        result *= term;
    }
    return result;
}
