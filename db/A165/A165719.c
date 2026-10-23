/*
 * A165719  Integers of the form k*(k+9)/8.
 * https://oeis.org/A165719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^2 + 24*n + 9 - 3*(-1)^n*(2*n+3)) / 4
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t two_n_plus3 = 2 * n_seq + 3;
    seq_t numerator = 8 * n_seq * n_seq + 24 * n_seq + 9;
    if (n & 1) {
        /* n odd: add 3*(2*n+3) */
        numerator += 3 * two_n_plus3;
    } else {
        /* n even: subtract 3*(2*n+3) */
        numerator -= 3 * two_n_plus3;
    }
    return numerator / 4;
}
