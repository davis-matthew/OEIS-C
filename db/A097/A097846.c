/*
 * A097846  Differences between A097598 and A045918.
 * https://oeis.org/A097846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   a(n) = 99 * ((n % 10) - (n / 10)) for n >= 10, a(n) = 0 for n <
 *            10
 * Range:     i8 n <= 12, i16 n <= 30, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10) return 0;

    idx_t tens   = n / 10;          /* floor division */
    idx_t units  = n % 10;

    /* Convert to seq_t before arithmetic to respect type rules */
    seq_t res = ((seq_t)units - (seq_t)tens) * (seq_t)99;
    return res;
}
