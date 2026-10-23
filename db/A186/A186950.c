/*
 * A186950  a(n) = n^2 - 47*n + 479.
 * https://oeis.org/A186950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*n - 47*n + 479
 * Range:     i8 none, i16 n <= 204, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x*x - (seq_t)47*x + (seq_t)479;
}
