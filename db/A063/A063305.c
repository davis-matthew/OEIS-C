/*
 * A063305  Dimension of the space S_n^{new}(Gamma_1(32)) of weight n cuspidal
 *          newforms for Gamma_1( 32 ).
 * https://oeis.org/A063305
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 18*n - 41 for n > 2, and a(2) = -1
 * Range:     i8 n <= 9, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: 2 (so a(2) is the first listed term) */

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)-1;
    return ((seq_t)18 * (seq_t)n) - (seq_t)41;
}
