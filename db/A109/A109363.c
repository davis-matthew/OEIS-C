/*
 * A109363  a(n) = 4*2^n - 3*n - 5.
 * https://oeis.org/A109363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*2^n - 3*n - 5
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n iteratively */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* combine according to the closed form */
    return (seq_t)4 * pow2 - (seq_t)3 * (seq_t)n - (seq_t)5;
}
