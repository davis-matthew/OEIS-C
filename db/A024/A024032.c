/*
 * A024032  a(n) = 3^n - n^9.
 * https://oeis.org/A024032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* Compute n^9 (9 is small and fits in idx_t) */
    seq_t pow9 = 1;
    seq_t sn   = (seq_t)n;
    for (int i = 0; i < 9; ++i)
        pow9 *= sn;

    return pow3 - pow9;
}
