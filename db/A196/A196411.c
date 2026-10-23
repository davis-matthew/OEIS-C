/*
 * A196411  a(n) = n! - n^4.
 * https://oeis.org/A196411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! - n^4
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    seq_t nseq = (seq_t)n;
    seq_t n4 = nseq * nseq * nseq * nseq;
    return fact - n4;
}
