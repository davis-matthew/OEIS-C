/*
 * A118881  Square of sum of decimal digits of n.
 * https://oeis.org/A118881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (sum of decimal digits of n)^2
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t) {
        sum += (seq_t)(t % 10);
        t /= 10;
    }
    return sum * sum;
}
