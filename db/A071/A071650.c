/*
 * A071650  Difference between sums of odd and even digits of n.
 * https://oeis.org/A071650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = (sum of odd digits of n) - (sum of even digits of n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t odd = 0;
    seq_t even = 0;
    idx_t x = n;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);   /* digit is small; cast to uint8_t */
        x /= 10;
        if (d & 1) {               /* odd digit */
            odd += (seq_t)d;
        } else {                   /* even digit */
            even += (seq_t)d;
        }
    }
    return odd - even;
}
