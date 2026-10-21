/*
 * A216853  18k^2-12k-7 interleaved with 18k^2+6k+5 for k>=0.
 * https://oeis.org/A216853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (6*n*(3*n-4)-27*(-1)^n-1)/4
 * Range:     i8 n <= 6, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent interleaved form: a(2k)=18k^2-12k-7, a(2k+1)=18k^2+6k+5 */
seq_t a(idx_t n) {
    seq_t k = (seq_t)(n >> 1);
    if ((n & 1) == 0) {
        return 18 * k * k - 12 * k - 7;
    } else {
        return 18 * k * k + 6 * k + 5;
    }
}
