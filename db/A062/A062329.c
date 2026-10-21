/*
 * A062329  a(n) = (sum of digits of n) - (product of digits of n).
 * https://oeis.org/A062329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy, look
 * Formula:   a(n) = (sum of digits of n) - (product of digits of n)
 * Range:     i8 n <= 298, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t tmp = n;
    while (tmp != 0) {
        int d = tmp % 10;
        sum += d;
        prod *= d;
        tmp /= 10;
    }
    return sum - prod;
}
