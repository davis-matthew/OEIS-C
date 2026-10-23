/*
 * A110892  Sum of the squares of digits of n^2.
 * https://oeis.org/A110892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of squares of the decimal digits of n^2
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;      /* compute n^2 in seq_t */
    seq_t sum = 0;
    while (sq > 0) {
        seq_t d = sq % (seq_t)10;        /* extract last decimal digit */
        sum += d * d;                    /* add its square */
        sq /= (seq_t)10;                 /* remove the digit */
    }
    return sum;
}
