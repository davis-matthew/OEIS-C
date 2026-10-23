/*
 * A110745  a(n) is a number such that if odd positioned digits are deleted
 *          one gets n and if even positioned digits are deleted one gets n
 *          reversed. Counting is from the LSB side. The position of LSB is
 *          one.
 * https://oeis.org/A110745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is formed by interleaving the digits of n with the * digits
 *            of reverse(n). Starting from the least significant digit *
 *            (position 1), we write the least significant digit of
 *            reverse(n), * then the least significant digit of n, then the
 *            next digit of * reverse(n), etc. This yields a number who...
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* extract digits of n from least‑significant to most‑significant */
    seq_t digits[20];
    idx_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        while (t > 0) {
            digits[len++] = (seq_t)(t % 10);
            t /= 10;
        }
    }

    /* build the interleaved number from most‑significant to least‑significant */
    seq_t result = 0;
    for (idx_t j = len; j > 0; ) {
        j--;                                 /* now j = len-1, ..., 0 */
        result = result * 10 + digits[j];     /* digit of n (from MSB side)   */
        result = result * 10 + digits[len-1-j];/* corresponding digit of reverse */
    }
    return result;
}
