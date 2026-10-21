/*
 * A065796  Alternating sum of digits of n^2.
 * https://oeis.org/A065796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = n^2 mod 10 - n^2 mod 100 div 10 + n^2 mod 1000 div 100 -
 *            ... (alternating sum of decimal digits of n^2)
 * Range:     i8 n <= 11, i16 n <= 233, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t sum = 0;
    seq_t sign = 1;
    while (sq != 0)
    {
        seq_t digit = sq % 10;
        sum += sign * digit;
        sq /= 10;
        sign = -sign;
    }
    return sum;
}
