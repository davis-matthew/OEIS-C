/*
 * A142705  Numerator of 1/4 - 1/(2n)^2.
 * https://oeis.org/A142705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (n^2-1)/gcd(n^2-1,4) = (n^2-1)/4 if n is odd = (n^2-1) if
 *            n is even Sequence: 0, 3, 2, 15, 6, 35, 12, 63, 20,
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t num = n_t * n_t - (seq_t)1;
    if (n & (idx_t)1) {         /* n odd */
        num /= (seq_t)4;
    }
    return num;
}
