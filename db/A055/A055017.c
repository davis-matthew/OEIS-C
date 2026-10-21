/*
 * A055017  Difference between sums of alternate digits of n starting with the
 *          last, i.e., (sum of ultimate digit of n, antepenultimate digit of
 *          n, ...) - (sum of penultimate digit of n, preantepenultimate digit
 *          of n, ...).
 * https://oeis.org/A055017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * (digit of n at position k), where k=0
 *            is the least‑significant digit (rightmost). This is the
 *            alternating digit sum starting with a positive sign for the last
 *            digit
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    int sign = 1;                 /* +1 for last digit, then switch */
    while (tmp > 0)
    {
        int digit = tmp % 10;     /* digit in 0..9 */
        sum += (seq_t)digit * sign;
        tmp /= 10;
        sign = -sign;             /* alternate sign */
    }
    return sum;
}
