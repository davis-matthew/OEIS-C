/*
 * A079399  Number of dots in Braille representation of n.
 * https://oeis.org/A079399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of dots for each decimal digit of n * dots[0..9] =
 *            {3,1,2,2,3,2,3,4,3,2}
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t dots[10] = {3,1,2,2,3,2,3,4,3,2};
    seq_t sum = 0;
    if (n == 0) return dots[0];
    idx_t tmp = n;
    while (tmp != 0) {
        idx_t d = tmp % 10;   /* decimal digit 0-9 */
        sum += dots[d];
        tmp /= 10;
    }
    return sum;
}
