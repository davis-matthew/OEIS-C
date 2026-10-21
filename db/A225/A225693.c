/*
 * A225693  Alternating sum of digits of n.
 * https://oeis.org/A225693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = d_0 - d_1 + d_2 - d_3 + ... where digits d_i are taken
 *            from most to least significant in base 10
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle zero explicitly */
    if (n == 0)
        return 0;

    /* Count digits to determine the initial sign */
    idx_t tmp = n;
    unsigned len = 0;
    while (tmp > 0) {
        len++;
        tmp /= 10;
    }

    /* Sign for the least significant digit is (-1)^(len-1) */
    int sign = ((len - 1) & 1) ? -1 : 1;
    seq_t result = 0;

    /* Process digits from least significant to most, alternating the sign */
    tmp = n;
    while (tmp > 0) {
        seq_t digit = (seq_t)(tmp % 10);
        result += (seq_t)sign * digit;
        tmp /= 10;
        sign = -sign;
    }

    return result;
}
