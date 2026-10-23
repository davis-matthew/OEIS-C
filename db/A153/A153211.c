/*
 * A153211  Sum of digits of n, times digital reversal of sum of digits of n.
 * https://oeis.org/A153211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = SOD(n) * reverse(SOD(n)) (sum of digits of n times its
 *            digit reversal)
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* reverse the sum */
    seq_t rev = 0;
    seq_t s = sum;
    while (s > 0) {
        rev = rev * 10 + s % 10;
        s /= 10;
    }

    return sum * rev;
}
