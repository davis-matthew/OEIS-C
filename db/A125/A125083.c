/*
 * A125083  a(n) = n^5-n^4-n^3-n^2-n-1.
 * https://oeis.org/A125083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5-n^4-n^3-n^2-n-1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 73, i64 n <= 580, i128 n <= 580,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t n5 = n4 * sn;
    return n5 - n4 - n3 - n2 - sn - 1;
}
