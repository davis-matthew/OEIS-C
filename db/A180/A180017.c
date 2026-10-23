/*
 * A180017  Difference of sums of digits of n in ternary and in binary.
 * https://oeis.org/A180017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = sum of ternary digits of n – sum of binary digits of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t     tmp;
    seq_t     sum3 = 0;
    seq_t     sum2 = 0;

    /* sum digits in base 3 */
    tmp = n;
    while (tmp) {
        sum3 += (seq_t)(tmp % 3);
        tmp /= 3;
    }

    /* sum digits in base 2 (i.e., popcount) */
    tmp = n;
    while (tmp) {
        sum2 += (seq_t)(tmp & 1);
        tmp >>= 1;
    }

    return sum3 - sum2;
}
