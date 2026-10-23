/*
 * A168052  Hankel transform of a Motzkin variant.
 * https://oeis.org/A168052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(2k) = k + k/3 + 1 * a(2k+1) = -( k + 1 + (k+2)/3 )
 * Range:     i8 n <= 191, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {             /* even n */
        idx_t k = n / 2;
        return (seq_t)(k + k / 3 + 1);
    } else {                        /* odd n */
        idx_t k   = n / 2;          /* floor((n-1)/2) */
        idx_t sum = k + 1 + (k + 2) / 3;
        return -(seq_t)sum;
    }
}
