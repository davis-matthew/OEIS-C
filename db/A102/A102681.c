/*
 * A102681  Number of digits >= 8 in decimal representation of n.
 * https://oeis.org/A102681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n that are >= 8
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 10;
        if (d >= 8) {
            cnt++;
        }
        m /= 10;
    }
    return cnt;
}
