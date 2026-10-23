/*
 * A128019  Expansion of (1 - 3x)/(1 + 3*x^2).
 * https://oeis.org/A128019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-3)^{floor((n+1)/2)}
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 38, i64 n <= 78, i128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t exp = (n + 1) >> 1;          /* floor((n+1)/2) */
    seq_t result = 1;
    seq_t mul = (seq_t)-3;
    for (idx_t i = 0; i < exp; ++i) {
        result *= mul;
    }
    return result;
}
